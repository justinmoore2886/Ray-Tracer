#include "Sphere.h"
#include <math.h>

#include <fstream>
#include <iostream>
using namespace std;
extern bool debugOn;

Sphere::Sphere(Point4& center, float radius, Material& color) : Object(color) {
    c = center;
    r = radius;
    name = "unnamed";
}

bool Sphere::intersects(Ray4& ray, Hit& hit) {
    float as = ray.direction * ray.direction;
    float bs = 2 * ray.direction * (ray.start - c);
    float cs = (ray.start - c)*(ray.start - c) - (r*r);

    float ds = bs*bs - 4*as*cs;
    float t_sphere = -1;
    Point4 P_sphere;
    Vector4 N;

    if (ds > EPSILON) {
        // Two real roots
        float t1 = (-bs - sqrt(ds)) / (2 * as);
        float t2 = (-bs + sqrt(ds)) / (2 * as);

        if (t1 > EPSILON) {
            t_sphere = t1;
        }
        else if (t2 > EPSILON) {
            t_sphere = t2;
        }
        else {
            return false;
        }
        P_sphere = ray.start + t_sphere * ray.direction;
        N = (P_sphere - c).normalized();

        hit.p = P_sphere;
        hit.N = N;
        hit.obj = this;
        hit.dist = ray.start.distanceTo(P_sphere);
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const Sphere& sphere) {
    os << "Sphere \"" << sphere.name << "\" center " << sphere.c
       << " radius " << sphere.r;
    return os;
}
