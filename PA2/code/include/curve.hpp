#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// TODO (PA2): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
};

class Curve : public Object3D {
    protected:
    std::vector<Vector3f> controls;
    public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray& r, Hit& h, float tmin) override {
        return false;
    }

    std::vector<Vector3f>& getControls() {
        return controls;
    }

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto& control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto& control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto& cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }
};


class BezierCurve : public Curve {
    public:
    explicit BezierCurve(const std::vector<Vector3f>& points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
    }

    int factorial(int n) {
        if (n == 0) return 1;
        return n * factorial(n - 1);
    }
    double bernstein(int i, int n, double t) {
        double binomial = 1;
        double C_n_i = factorial(n) / (factorial(i) * factorial(n - i));
        return C_n_i * std::pow(t, i) * std::pow(1 - t, n - i);
    }

    Vector3f bezier(double t, int step, int k) {
        Vector3f point;
        for (int i = 0; i <= k; i++) {
            point += controls[step * 3 + i] * bernstein(i, k, t);
        }
        return point;
    }

    Vector3f beziertan(double t, int step, int k) {
        Vector3f tan;
        for (int i = 0; i <= k - 1; i++) {
            double bias = bernstein(i, k - 1, t);
            tan += (controls[step * 3 + i + 1] - controls[step * 3 + i]) * bias;
        }
        return tan * (controls.size() - 1);
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();

        double step = 1.0 / resolution;
        int totalSteps = (controls.size() - 1) / 3;
        int k = 3;
        for (int i = 0; i < totalSteps; i++) {
            for (double t = 0; t < 1.0; t += step) {
                Vector3f V = bezier(t, i, k);
                Vector3f T = beziertan(t, i, k);
                T.normalize();
                data.push_back({ V, T });
            }
        }
    }

    protected:

};

class BsplineCurve : public Curve {
    public:
    BsplineCurve(const std::vector<Vector3f>& points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
    }

    double Bspline(int i, int p, double u, const std::vector<double>& knots) {
        if (p == 0) {
            if (u >= knots[i] && u < knots[i + 1]) {
                return 1.0;
            }
            else {
                return 0.0;
            }
        }

        // 递归计算B样条基函数
        double firstPart = 0.0;
        double secondPart = 0.0;

        double denominator1 = knots[i + p] - knots[i];
        double denominator2 = knots[i + p + 1] - knots[i + 1];

        if (denominator1 != 0.0) {
            firstPart = (u - knots[i]) / denominator1 * Bspline(i, p - 1, u, knots);
        }
        if (denominator2 != 0.0) {
            secondPart = (knots[i + p + 1] - u) / denominator2 * Bspline(i + 1, p - 1, u, knots);
        }

        return firstPart + secondPart;
    }

    Vector3f BsplinePoint(int k, double t, const std::vector<double>& knots) {
        Vector3f point;
        for (int j = 0; j < controls.size(); j++) {
            point += controls[j] * Bspline(j, k, t, knots);
        }
        return point;
    }
    Vector3f BsplineTan(int k, double t, const std::vector<double>& knots) {
        Vector3f tan;
        for (int i = 1; i < controls.size(); i++) {
            tan += (k - 1) * (controls[i] - controls[i - 1]) * Bspline(i, k - 1, t, knots) / (knots[i + k - 1] - knots[i]);
        }
        //std::cout << tan;
        return tan;
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA2): fill in data vector
        std::vector<double> knots;
        int k = 3;
        for (int i = 0; i <= controls.size() + k + 1; i++) {
            double t = double(i) / (controls.size() + k + 1);
            knots.push_back(t);
        }
        double start = knots[k];
        double end_point = knots[controls.size()];
        for (int i = k; i < controls.size(); i++) {
            double start = knots[i];
            double end_point = knots[i + 1];
            double step = (end_point - start) / resolution;
            for (double t = start; t < end_point; t += step) {
                Vector3f V = BsplinePoint(3, t, knots);
                Vector3f T = BsplineTan(3, t, knots);
                T.normalize();
                data.push_back({ V, T });
            }
        }
    }

    protected:

};

#endif // CURVE_HPP
