#include "Light.h"

Light::Light() {
    c.set(1,1,1);
    p.set(0,0,0);
    name = "unnamed";
}

Light::Light(Color& color, Point4& position) {
    c = color;
    p = position;
}

void Light::set(Color& color, Point4& position) {
    c = color;
    p = position;
}

ostream& operator<<(ostream& os, const Light& light) {
    os << "Light \"" << light.name << "\" color " << light.c
       << " at " << light.p;
    return os;
}
