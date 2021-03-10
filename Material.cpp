#include "Material.h"

Material::Material() {
    ka.set(.5, .5, .5);
    kd.set(.5, .5, .5);
    ks.set(.5, .5, .5);
    n = 10;
    rho.set(.5, .5, .5);
    tau.set(.5, .5, .5);
    surface_type = PHONG;
}

Material::Material(Color& ambient, Color& diffuse, Color& specular, int shininess) {
    set(ambient, diffuse, specular, shininess);
}

void Material::set(Color& ambient, Color& diffuse, Color& specular, int shininess) {
    ka = ambient;
    kd = diffuse;
    ks = specular;
    n  = shininess;
    surface_type = PHONG;
}


Material::Material(float refraction, Color& transmission, Color& reflection, Color& color) {
    set(refraction, transmission, reflection, color);
}

void Material::set(float refraction, Color& transmission, Color& reflection, Color& c) {
    refraction_index = refraction;
    tau = transmission;
    rho = reflection;
    color = c;
    is_reflective  = true; // ???
    is_translucent = true;
    surface_type = SPECULAR;
}

ostream& operator<<(ostream& os, const Material& mat) {
    os << "Material \"" << mat.name << "\"";
    if (mat.surface_type == PHONG) {
        os << " PHONG\n";
        os << "  ka " << mat.ka << "\n";
        os << "  kd " << mat.kd << "\n";
        os << "  ks " << mat.ks << "\n";
        os << "  shininess " << mat.n << "\n";
    }
    else {
        os << " SPECULAR\n";
        os << "  tau " << mat.tau << (mat.is_translucent?"translucent\n":"\n");
        os << "  rho " << mat.rho << (mat.is_reflective ?"reflective\n":"\n");
        os << "  clr " << mat.color << "\n";
        os << "  n   " << mat.refraction_index << "\n";
    }
    return os;
}
