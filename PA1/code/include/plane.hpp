#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
    public:
    Plane() {

    }

    Plane(const Vector3f& normal, float d, Material* m) : Object3D(m) {
        this->material = m;
        this->normal = normal;
        this->d = -1 * d;
    }

    ~Plane() override = default;

    bool intersect(const Ray& r, Hit& h, float tmin) override {
        float t = -1 * (d + Vector3f::dot(normal, r.getOrigin())) / Vector3f::dot(normal, r.getDirection());
        if (t > tmin && t < h.getT()) {
            h.set(t, this->material, normal.normalized());
            return true;
        }
        return false;
    }

    protected:
    Vector3f normal;
    float d;

};

#endif //PLANE_H


