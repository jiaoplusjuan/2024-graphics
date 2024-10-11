#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
    public:
    Sphere() {
        center = Vector3f(0, 0, 0); // default: unit ball at the center
        radius = 1.0f;
        material = nullptr;
        // unit ball at the center
    }

    Sphere(const Vector3f& center, float radius, Material* material) : Object3D(material) {
        this->center = center;
        this->radius = radius;
        this->material = material;
    }

    ~Sphere() override = default;

    bool intersect(const Ray& r, Hit& h, float tmin) override { // 避免与光线起点非常接近的物体相交
        Vector3f L = center - r.getOrigin();
        float Tp = Vector3f::dot(L, r.getDirection());
        float distance = L.length();
        float d = sqrt(distance * distance - Tp * Tp);// 计算球心到这条射线的距离
        if (distance > radius && Tp < 0) {
            // h.set(tmin, material, Vector3f(0, 0, 0));
            return false;
        } //光源在球体外部且Tp小于0
        if (d > radius) {
            return false;
        } //光线不与球体相交
        float d2 = sqrt(radius * radius - d * d); // 计算球心到交点的距离
        if (distance > radius) { //光源在球体外部
            float t = Tp - d2;
            Vector3f intersection = r.getOrigin() + r.getDirection() * t; // 计算交点坐标
            Vector3f normal = (intersection - center).normalized(); // 计算法向量
            if (t < h.getT() && t >= tmin) {
                h.set(t, material, normal);
                return true;
            }
            else {
                return false;
            }
        }
        else { //光源在球体内部
            float t = Tp + d2;
            Vector3f intersection = r.getOrigin() + r.getDirection() * t; // 计算交点坐标
            Vector3f normal = (intersection - center).normalized(); // 计算法向量
            if (t < h.getT() && t >= tmin) {
                h.set(t, material, normal);
                return true;
            }
            else {
                return false;
            }
        }
        return false;
    }

    protected:
    Vector3f center;
    float radius;
};


#endif
