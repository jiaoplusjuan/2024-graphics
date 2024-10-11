#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


class Group : public Object3D {

    public:

    Group() {

    }

    explicit Group(int num_objects) {
        objects.resize(num_objects);
    }

    ~Group() override {
        objects.clear();
    }

    bool intersect(const Ray& r, Hit& h, float tmin) override {
        bool isIntersect = false;
        for (Object3D* obj : objects) {
            if (obj->intersect(r, h, tmin)) {
                isIntersect = true;
            }
        }
        return isIntersect;
    }

    void addObject(int index, Object3D* obj) {
        objects[index] = obj;
    }

    int getGroupSize() {
        return objects.size();
    }

    void drawGL() {
        for (Object3D* obj : objects) {
            obj->drawGL();
        }
    }
    private:
    std::vector<Object3D*> objects;
};

#endif

