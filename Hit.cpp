#include "Hit.h"

Hit::Hit() {
    p.set(0,0,0);
    N.set(1,0,0);
    obj = 0;
    dist = 0;
}

Hit::Hit(Point4& hitpoint, Vector4& normal, Object* obj, double d) {
    p = hitpoint;
    N = normal;
    this->obj = obj;
    dist = d;
}

void Hit::set(Point4& hitpoint, Vector4& normal, Object* obj, double d) {
    p = hitpoint;
    N = normal;
    this->obj = obj;
    dist = d;
}

