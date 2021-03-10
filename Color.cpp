#include "Color.h"

Color::Color() {
    set(0,0,0);
}

Color::Color(float red, float green, float blue) {
    Float4::set(red,green,blue, 1);
}

void Color::set(float red, float green, float blue) {
    Float4::set(red,green,blue, 1);
}

void Color::clamp()
{
    if (R() < 0) R() = 0;
    if (R() > 1) R() = 1;

    if (G() < 0) G() = 0;
    if (G() > 1) G() = 1;

    if (B() < 0) B() = 0;
    if (B() > 1) B() = 1;
}

// Assignment.
Color& Color::operator=(const Color& other) {
    copyFrom(other);
    return *this;
};

Color& Color::operator+=(const Color& other) {
    if (&other != this) {
        plus(other, *this);
    }

    return (*this);
}

Color& Color::operator*=(const Color& other) {
    if (&other != this) {
        set(R() * other.R(),
            G() * other.G(),
            B() * other.B());
    }

    return (*this);
}

Color Color::operator*(const Color& other) const {
    Color result;
    result.set(R() * other.R(),
               G() * other.G(),
               B() * other.B());
    return result;
}

Color Color::operator+(const Color& other) const {
    Color result;
    result.set(R() + other.R(),
               G() + other.G(),
               B() + other.B());
    return result;
}

Color Color::operator*(float scalar) const {
    Color result;
    result.set(R() * scalar,
               G() * scalar,
               B() * scalar);
    return result;
}

// Named access to the four components: L-values

float& Color::R() {
    return v[0];
}

float& Color::G() {
    return v[1];
}

float& Color::B() {
    return v[2];
}

// Named access to the components: R-values

const float& Color::R() const {
    return v[0];
}

const float& Color::G() const {
    return v[1];
}

const float& Color::B() const {
    return v[2];
}

