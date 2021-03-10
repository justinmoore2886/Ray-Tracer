#if !defined(_OBJECT_H_)

#define _OBJECT_H_

#include <string>

#include "Material.h"
#include "Hit.h"
#include "GeomLib.h"

enum ObjectType {NO_OBJECT, SPHERE, TRIANGLE};

class Object {
public:
    Object(Material& newColor);
    virtual bool intersects(Ray4& ray, Hit& hit) = 0;
    Material& getMaterial() {return material;};

//#protected:
    Material material;
    string name;

};

#endif
