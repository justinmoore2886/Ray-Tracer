#if !defined(_MATERIAL_H_)

#define _MATERIAL_H_

#include <string>
#include "Color.h"

enum SurfaceType {NO_SURFACE, PHONG, SPECULAR};

class Material {
public:
    Material();
    Material(Color& ambient, Color& diffuse, Color& specular, int shininess);
    void set(float refraction_index, Color& transmission, Color& reflection, Color& color);
    Material(float refraction_index, Color& transmission, Color& reflection, Color& color);
    void set(Color& ambient, Color& diffuse, Color& specular, int shininess);

    inline Color& getAmbient() { return ka; };
    inline Color& getDiffuse() { return kd; };
    inline Color& getSpecular() { return ks; };
    inline Color& getTransmission() { return tau; };
    inline Color& getReflection() { return rho; };
    inline int getShininess() { return n; };
    inline SurfaceType getType() { return surface_type; };

    friend ostream& operator<<(ostream& os, const Material& mat);

//private:

    string name;

    SurfaceType surface_type;
    // For Phong glossy materials
    Color ka,kd,ks;
    int n;

    // For specular materials
    Color tau, rho, color;
    bool is_translucent;
    bool is_reflective;
    float refraction_index;
};

#endif
