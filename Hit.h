#if !defined(_HIT_H_)
#define _HIT_H_

#include "GeomLib.h"

class Object;

class Hit {
public:
    Hit();
    Hit(Point4& hitpoint, Vector4& normal, Object* obj, double d);
    void set(Point4& hitpoint, Vector4& normal, Object* obj, double d);
    Point4& hitPoint() {return p;};
    Vector4& normal() {return N;};
    Object* getObject() {return obj;};
    double getDistance() {return dist;};

    // private:
    Point4 p;
    Vector4 N;
    Object* obj;
    double dist;
};

#endif
