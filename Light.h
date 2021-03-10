#if !defined(_LIGHT_H_)
#define _LIGHT_H_

#include <string>

#include "GeomLib.h"
#include "Color.h"

class Light {
public:
    Light();
    Light(Color& color, Point4& position);
    void set(Color& color, Point4& position);
    Color& getColor() {return c;};
    Point4& getPos() {return p;};

    string name;
    Point4 p;
    Color c;

    friend ostream& operator<<(ostream& os, const Light& light);
};

#endif
