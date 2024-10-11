#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
    public:
    Camera(const Vector3f& center, const Vector3f& direction, const Vector3f& up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f& point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// TODO: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {

    public:
    PerspectiveCamera(const Vector3f& center, const Vector3f& direction,
        const Vector3f& up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        this->angle = angle;
        // angle is in radian.
    }

    Ray generateRay(const Vector2f& point) override {
        float fx = height / (2 * tan(angle / 2.0f));
        float fy = fx;
        float cx = width / 2.0f;
        float cy = height / 2.0f;
        float x = (point.x() - cx) / fx;
        float y = (cy - point.y()) / fy;
        Matrix3f R = Matrix3f(horizontal, -1 * up, direction);
        Vector3f normal = R * (Vector3f(x, y, 1).normalized());
        return Ray(center, normal);
        // 
    }
    protected:
    float angle;
};

#endif //CAMERA_H
