#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <glut.h>

class Material {
    public:

    explicit Material(const Vector3f& d_color, const Vector3f& s_color = Vector3f::ZERO, float s = 0) :
        diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }


    Vector3f Shade(const Ray& ray, const Hit& hit,
        const Vector3f& dirToLight, const Vector3f& lightColor) {// dirToLigh和ray相反
        Vector3f shaded = Vector3f::ZERO;
        float nDotL = Vector3f::dot(hit.getNormal(), dirToLight);
        Vector3f diffuse = diffuseColor * (nDotL > 0 ? nDotL : 0);
        Vector3f r = 2 * (Vector3f::dot(hit.getNormal(), dirToLight)) * hit.getNormal() - dirToLight;
        float rDotV = Vector3f::dot(r, -1 * ray.getDirection());
        Vector3f specular = specularColor * (rDotV > 0 ? pow(rDotV, shininess) : 0);
        shaded = diffuse + specular;
        return shaded * lightColor;
    }

    // For OpenGL, this is fully implemented
    void Use() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f));
    }

    protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
};


#endif // MATERIAL_H
