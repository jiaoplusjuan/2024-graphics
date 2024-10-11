#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <stack>
class Element {
    public:
    virtual void draw(Image& img) = 0;
    virtual ~Element() = default;
};

class Line : public Element {

    public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image& img) override {
        //Bresenham's line algorithm
        bool kIsPositive = (yB - yA) * (xB - xA) >= 0;
        bool kBiggerThan1 = std::abs(yB - yA) > std::abs(xB - xA);
        if (kIsPositive) {
            xA = std::min(xA, xB);
            yA = std::min(yA, yB);
            xB = std::max(xA, xB);
            yB = std::max(yA, yB);
            int dx = xB - xA, dy = yB - yA, e = -1 * dx;
            int x = xA, y = yA;
            if (kBiggerThan1) {
                for (int i = 0; i <= dy;i++) {
                    img.SetPixel(x, y, color);
                    y++;
                    e += 2 * dx;
                    if (e > 0) {
                        x++;
                        e -= 2 * dy;
                    }
                }
            }
            else {
                for (int i = 0; i <= dx; i++) {
                    img.SetPixel(x, y, color);
                    x++;
                    e += 2 * dy;
                    if (e > 0) {
                        y++;
                        e -= 2 * dx;
                    }
                }
            }
        }
        else {
            xA = std::max(xA, xB);
            xB = std::min(xA, xB);
            yA = std::min(yA, yB);
            yB = std::max(yA, yB);
            int dx = xA - xB, dy = yB - yA, e = -1 * dx;
            int x = xA, y = yA;
            if (kBiggerThan1) {
                for (int i = 0; i <= dy;i++) {
                    img.SetPixel(x, y, color);
                    y++;
                    e += 2 * dx;
                    if (e > 0) {
                        x--;
                        e -= 2 * dy;
                    }
                }
            }
            else {
                for (int i = 0; i <= dx; i++) {
                    img.SetPixel(x, y, color);
                    x--;
                    e += 2 * dy;
                    if (e > 0) {
                        y++;
                        e -= 2 * dx;
                    }
                }
            }
        }
        // TODO: Implement Bresenham Algorithm
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n", xA, yA, xB, yB,
            color.x(), color.y(), color.z());
    }
};

class Circle : public Element {

    public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image& img) override {
        int x = cx, y = cy + radius;
        float d = 1.25 - radius;
        circlePoints(x, y, img);
        while ((x - cx) <= (y - cy)) {
            if (d < 0) {
                d += 2 * (x - cx) + 3;
            }
            else {
                d += 2 * (x - cx) - 2 * (y - cy) + 5;
                y--;
            }
            x++;
            circlePoints(x, y, img);
        }
        // TODO: Implement Algorithm to draw a Circle
        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n", cx, cy, radius,
            color.x(), color.y(), color.z());
    }
    void circlePoints(int x, int y, Image& img) {
        img.SetPixel(x, y, color);
        img.SetPixel(2 * cx - x, y, color);
        img.SetPixel(x, 2 * cy - y, color);
        img.SetPixel(2 * cx - x, 2 * cy - y, color);
        img.SetPixel(y - cy + cx, x - cx + cy, color);
        img.SetPixel(2 * cx - (y - cy + cx), x - cx + cy, color);
        img.SetPixel(y - cy + cx, 2 * cy - (x - cx + cy), color);
        img.SetPixel(2 * cx - (y - cy + cx), 2 * cy - (x - cx + cy), color);
    }
};

struct Point {
    int x;
    int y;
};

class Fill : public Element {

    public:
    int cx, cy;
    Vector3f color;
    int x, y, xl, xr;
    void draw(Image& img) override {
        //TODO: Flood fill
        Point pt = { cx, cy };
        std::stack<Point> myStack;
        myStack.push(pt);
        bool spanNeedFill;
        Vector3f oldColor = img.GetPixel(cx, cy);
        while (!myStack.empty()) {
            Point p = myStack.top();
            myStack.pop();
            x = p.x;
            y = p.y;
            while (x < img.Width() && img.GetPixel(x, y) == oldColor) { // 向右填充
                img.SetPixel(x, y, color);
                x++;
            }
            xr = x - 1;// 记录右边界
            x = p.x - 1;
            while (x >= 0 && img.GetPixel(x, y) == oldColor) {
                img.SetPixel(x, y, color);
                x--;
            }
            xl = x + 1; // 记录左边界
            // 处理上面的线
            x = xl;
            y = y + 1;
            if (y < img.Height()) {
                while (x <= xr) {
                    spanNeedFill = false;
                    while (x < img.Width() && img.GetPixel(x, y) == oldColor) {
                        spanNeedFill = true;
                        x++;
                    }
                    if (spanNeedFill) {
                        pt.x = x - 1;
                        pt.y = y;
                        myStack.push(pt);
                        spanNeedFill = false;
                    }
                    while (x < img.Width() && img.GetPixel(x, y) != oldColor && x <= xr) {
                        x++;
                    }
                }
            }
            x = xl;
            y = y - 2;
            if (y >= 0) {
                while (x <= xr) {
                    spanNeedFill = false;
                    while (x < img.Width() && img.GetPixel(x, y) == oldColor) {
                        spanNeedFill = true;
                        x++;
                    }
                    if (spanNeedFill) {
                        pt.x = x - 1;
                        pt.y = y;
                        myStack.push(pt);
                        spanNeedFill = false;
                    }
                    while (x < img.Width() && img.GetPixel(x, y) != oldColor && x <= xr) {
                        x++;
                    }
                }
            }
        }
        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
            color.x(), color.y(), color.z());
    }
};