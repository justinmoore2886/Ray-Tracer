#include "Triangle.h"

#include <math.h>
#include <fstream>
#include <iostream>
using namespace std;
extern bool debugOn;

#define EPSILON 0.00001

Triangle::Triangle(Point4& v1, Point4& v2, Point4& v3, Material& color) : Object(color) {
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
    name = "unnamed";

    setNormal();
}

void Triangle::setNormal() {
    Vector4 edge1 = v2 - v1;
    Vector4 edge2 = v3 - v1;

    N = (edge1 ^ edge2).normalized();

    if (debugOn) {
        cout << "Triangle.setNormal N=" << N << "\n";
    }
}

bool Triangle::intersects(Ray4& ray, Hit& hit) {
    Vector4 V = ray.direction;
    Point4 S = ray.start;
    Point4 A = v1;
    Point4 B = v2;
    Point4 C = v3;

    float a = V.X(), b = A.X() - B.X(), c = A.X() - C.X(), k = A.X() - S.X();
    float d = V.Y(), e = A.Y() - B.Y(), f = A.Y() - C.Y(), l = A.Y() - S.Y();
    float g = V.Z(), h = A.Z() - B.Z(), j = A.Z() - C.Z(), m = A.Z() - S.Z();

    // Cramer's rule
    float denom = Matrix4::det3x3(a,b,c,
                                  d,e,f,
                                  g,h,j);

    float t = Matrix4::det3x3(k,b,c,
                              l,e,f,
                              m,h,j) / denom;
    float u = Matrix4::det3x3(a,k,c,
                              d,l,f,
                              g,m,j) / denom;
    float v = Matrix4::det3x3(a,b,k,
                              d,e,l,
                              g,h,m) / denom;

    Point4 P_triangle;

    if (0 <= u && u <= 1 &&
        0 <= v && v <= 1 &&
        0 <= u+v && u+v <= 1
        && EPSILON <= t) {
        P_triangle = S + t * V;

        hit.p = P_triangle;
        hit.N = N;
        hit.obj = this;
        hit.dist = ray.start.distanceTo(P_triangle);
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const Triangle& triang) {
    os << "Triangle \"" << triang.name << "\" v1 " << triang.v1
       << " v2 " << triang.v2 << " v3 " << triang.v3;
    return os;
}
