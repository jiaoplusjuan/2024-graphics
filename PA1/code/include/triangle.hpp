#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle : public Object3D {

	public:
	Triangle() = delete;

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = a;
		this->vertices[1] = b;
		this->vertices[2] = c;
		this->normal = (Vector3f::cross(b - a, c - a)).normalized();
		this->material = m;
	}

	bool intersect(const Ray& ray, Hit& hit, float tmin) override {
		Vector3f E1 = vertices[0] - vertices[1];
		Vector3f E2 = vertices[0] - vertices[2];
		Vector3f S = vertices[0] - ray.getOrigin();
		Matrix3f M(ray.getDirection(), E1, E2);
		float det1 = M.determinant();
		if (det1 == 0) {
			return false;
		}
		Matrix3f M2(S, E1, E2), M3(ray.getDirection(), S, E2), M4(ray.getDirection(), E1, S);
		float t = M2.determinant() / det1;
		float u = M3.determinant() / det1;
		float v = M4.determinant() / det1;
		if (t > tmin && t < hit.getT() && u >= 0 && v >= 0 && u + v <= 1) {
			hit.set(t, this->material, normal);
			return true;
		}
		return false;
	}
	Vector3f normal;
	Vector3f vertices[3];
	protected:
};

#endif //TRIANGLE_H
