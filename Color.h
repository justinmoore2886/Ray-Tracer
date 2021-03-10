#if !defined(_COLOR_H_)
#define _COLOR_H_

#include "GeomLib.h"

class Color : public Float4 {
public:
    Color();
    Color(float red, float green, float blue);
    void set(float red, float green, float blue);
    void clamp();

    // Assignment.
    Color& operator=(const Color& r);

    // add a color to this
    Color& operator+=(const Color& r);

    // multiply a color into this
    Color& operator*=(const Color& r);

    // scale this color
    Color& operator*=(float amount);

    // scale this color by reciprocal
    Color& operator/=(float amount);

    // Add two colors
    Color operator+(const Color& other) const;

    // Component-wise product of two colors.
    Color operator*(const Color& other) const;

    // Color-and-scalar arithmetic operations
    //
    // vector * scalar

    Color operator*(float factor) const;

    //
    // scalar * color
    //

    friend Color operator*(float factor, const Color& other);

    // divide (scale by reciprocal)
    Color operator/(float factor) const;

    // Assignment.
    Color& operator=(const Float4& r);

    // Named access to the three components: L-values

    float& R();
    float& G();
    float& B();

    // Named access to the components: R-values

    const float& R() const;
    const float& G() const;
    const float& B() const;
};

#endif
