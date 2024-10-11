#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>


class Plane : public Object3D {
    public:
    Plane() {

    }

    Plane(const Vector3f& normal, float d, Material* m) : Object3D(m) {
        this->material = m;
        this->norm = normal;
        this->d = d;
    }

    ~Plane() override = default;

    bool intersect(const Ray& r, Hit& h, float tmin) override {
        float t = -1 * (d + Vector3f::dot(norm, r.getOrigin())) / Vector3f::dot(norm, r.getDirection());
        if (t > tmin && t < h.getT()) {
            h.set(t, this->material, norm.normalized());
            return true;
        }
        return false;
    }

    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(norm, xAxis);
        xAxis = Vector3f::cross(yAxis, norm);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * norm + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(norm);
        glVertex3fv(d * norm + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * norm - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

    protected:
    Vector3f norm;
    float d;

};

#endif //PLANE_H


