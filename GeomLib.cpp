#include "GeomLib.h"

//
// Default constructor
//
Float4::Float4()
{
    set(0,0,0,1);
}

//
// Explicit constructor
//
Float4::Float4(const float x, const float y, const float z, const float w)
{
    set(x,y,z,w);
}

//
// Copy constructor
//
Float4::Float4(const Float4 &r)
{
    v[0] = r.v[0];
    v[1] = r.v[1];
    v[2] = r.v[2];
    v[3] = r.v[3];
}

//
// Intialize all 4 components
//
void Float4::set(float x, float y, float z, float w) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
}

Point4 Float4::homogenized()
{
    Point4 result;
    homogenize(result);
    return result;
}

// Compare for equality

bool Float4::operator==(const Float4& other)
{
    float dx = v[0] - other[0];
    float dy = v[1] - other[1];
    float dz = v[2] - other[2];
    float dw = v[3] - other[3];

    // two points are considered equal if
    // they are close enough; exact comparisons of floats is unwise

    float sum = fabs(dx) + fabs(dy) + fabs(dz) + fabs(dw);
    return sum < EPSILON;
}

// Access internal components of point's data array.

// L-value (on left of = sign)

float& Float4::operator [](int index) {
    assert (0 <= index && index <= 3);
    return v[index];
}

// R-value (on right of = sign)

const float& Float4::operator [](int index) const {
    assert (0 <= index && index <= 3);
    return v[index];
}

// Named access to the four components: L-values

float& Float4::X() {
    return v[0];
}
float& Float4::Y() {
    return v[1];
}
float& Float4::Z() {
    return v[2];
}
float& Float4::W() {
    return v[3];
}

// Named access to the components: R-values

const float& Float4::X() const {
    return v[0];
}
const float& Float4::Y() const {
    return v[1];
}
const float& Float4::Z() const {
    return v[2];
}
const float& Float4::W() const {
    return v[3];
}

//
// Output to stream
//

string format_number(double x, int width, int precision) {
    if (width > 99)
        width = 99;
    char output[100];
    char format[10];
    int i_x = int(x);
    if (fabs(i_x - x) < EPSILON) {
        sprintf(format, "%%%dd", width);
        sprintf(output, format, i_x);
    }
    else {
        sprintf(format, "%%%d.%df", width, precision);
        sprintf(output, format, x);
    }

    // cout << "x: " <<  x << " i_x: " <<  i_x << " width: " << width
    //      << " format: " << format << " out: " << output << "\n";
    return string(output);
}

ostream& operator<<(ostream& os,const Float4& m) {
    out_delimited(os, m, '[', ']');
    return os;
}

void out_delimited(ostream& os, const Float4& m,
                       char l_delim, char r_delim) {
    os << l_delim << format_number(m.v[0], 6, 2);
    os << " " << format_number(m.v[1], 6, 2);
    os << " " << format_number(m.v[2], 6, 2);
    os << " " << format_number(m.v[3], 6, 2) << r_delim;
}



// Read from stream

istream& operator>>(istream& is, Float4& p) {
    for (int c=0;c<4;c++) {
        is >> p[c];
    }
    return is;
}

//
// Default constructor
//
Point4::Point4() {
    set(0,0,0);
}

//
// Explicit constructor
//
Point4::Point4(const float x, const float y, const float z) {
    set(x,y,z);
}

//
// Explicit constructor
//
void Point4::set(float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1.0f;
}

// Assignment.
Point4& Point4::operator=(const Float4& r) {
    copyFrom(r);
    return *this;
};

// Add a vector to this point
Point4& Point4::operator+=(const Vector4& r) {
    this->plus(r, *this);
    return *this;
}

// Binary arithmetic operations

// Sum of this point and a vector
Point4 Point4::operator+(const Vector4& other) const {
    Point4 result;
    this->plus(other, result);
    return result;
}

// Subtract a vector from this point
Point4 Point4::operator-(const Vector4& other) const {
    Point4 result;
    this->minus(other, result);
    return result;
}

// Difference of two points is a vector
Vector4 Point4::operator-(const Point4& other) const {
    Vector4 result;
    this->minus(other, result);
    return result;
}

//
// Default constructor
//
Vector4::Vector4() {
    set(0,0,0);
}

//
// Explicit constructor
//
Vector4::Vector4(float x, float y, float z) {
    set(x,y,z);
}

void Vector4::set(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 0.0f;
}

//Vector4::operator const Float4  () { return *this; } const //conversion operator
//Vector4::operator const Float4& () { return *this; } const //conversion operator

// Assignment.
Vector4& Vector4::operator=(const Float4& r) {
    copyFrom(r);
    return *this;
}

// subtract a vector from this
Vector4& Vector4::operator-=(const Vector4& r) {
    this->minus(r, *this);
    return *this;
}

// add a vector to this
Vector4& Vector4::operator+=(const Vector4& r) {
    this->plus(r, *this);
    return *this;
}

// scale this vector
Vector4& Vector4::operator*=(float amount) {
    this->times(amount, *this);
    return *this;
}

// scale this vector by reciprocal
Vector4& Vector4::operator/=(float amount) {
    assert(amount != 0);
    this->times(1.0f / amount, *this);
    return *this;
}

// Unary arithmetic operations.

// Negate (flip) this vector
Vector4 Vector4::operator-() const {
    Vector4 result;
    this->times(-1.0f, result);
    return result;
}

// Binary arithmetic operations

// Add two vectors
Vector4 Vector4::operator+(const Vector4& other) const {
    Vector4 result;
    this->plus(other, result);
    return result;
}

// subtract two vectors
Vector4 Vector4::operator-(const Vector4& other) const {
    Vector4 result;
    this->minus(other, result);
    return result;
}

// add a point: yields a point
Point4 Vector4::operator+(const Point4& other) const {
    Point4 result;
    this->plus(other, result);
    return result;
}

// Vector4-and-scalar arithmetic operations
//
// vector * scalar

Vector4 Vector4::operator*(float factor) const {
    Vector4 result;
    this->times(factor,result);
    return result;
}

//
// scalar * vector
//

Vector4 operator*(float factor, const Vector4& other) {
    return other * factor;
}

// divide (scale by reciprocal)
Vector4 Vector4::operator/(float factor) const {
    assert(factor != 0);
    Vector4 result;
    this->times(1.0f / factor, result);
    return result;
}

//
// Returns a normalized version of this vector.
//
Vector4 Vector4::normalized() const {
    Vector4 result;
    this->normalize(result);
    return result;
}

//
// Returns dot product of this and other.
// * operator is DOT product !!!!!
//
float Vector4::operator*(const Vector4& other) const {
    return dot(other);
}

//
// Returns cross product of this and other.
// ^ operator is CROSS product !!!!
//
Vector4 Vector4::operator^(const Vector4& other) const {
    Vector4 result;
    this->cross(other, result);
    return result;
}

//
// Default constructor: set to identity matrix.
//
Matrix4::Matrix4() {
    set(1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1);
}

//
// Explicit constructor.
//

Matrix4::Matrix4(float xx, float xy, float xz, float xw,
                 float yx, float yy, float yz, float yw,
                 float zx, float zy, float zz, float zw,
                 float wx, float wy, float wz, float ww)
{
    set(xx,xy,xz,xw,
        yx,yy,yz,yw,
        zx,zy,zz,zw,
        wx,wy,wz,ww);
}

//
// Initialize all 16 components.
//

void Matrix4::set(float xx, float xy, float xz, float xw,
                  float yx, float yy, float yz, float yw,
                  float zx, float zy, float zz, float zw,
                  float wx, float wy, float wz, float ww)
{
    m[0].set(xx,xy,xz,xw);
    m[1].set(yx,yy,yz,yw);
    m[2].set(zx,zy,zz,zw);
    m[3].set(wx,wy,wz,ww);
}

//
// Copy Constructor.
//

Matrix4::Matrix4(const Matrix4& other)
{
    m[0] = other.m[0];
    m[1] = other.m[1];
    m[2] = other.m[2];
    m[3] = other.m[3];
}

//
// Set a single entry.
//
void Matrix4::set(int row, int col, float value)
{
    assert(0 <= row && row <= 3 &&
           0 <= col && col <= 3);
    m[row][col] = value;
}

//
// Array-access operators
//

Float4& Matrix4::operator[](int index)
{
    assert(0 <= index && index <= 3);
    return m[index];
}

const Float4& Matrix4::operator [](int index) const
{
    assert (0 <= index && index <= 3);
    return m[index];
}

//
// Unary operations
//

Matrix4 Matrix4::transpose() const {
    Matrix4 result;
    transpose(result);
    return result;
}

Matrix4 Matrix4::operator-() const {
    Matrix4 result;
    this->times(-1.0f, result);
    return result;
}

/*------------------------------------------------------*/
/*
 * Invert a 4x4 matrix.  Adapted from Richard Carling's code
 * in "Graphics Gems I".
 */

#define SMALL_NUMBER    (1.e-8)

Matrix4 Matrix4::inverse() const {
    Matrix4 result;

    result = adjoint();

    float det = determinant();

    if ( fabs( det ) < SMALL_NUMBER) {
        cerr << "(Matrix4::inverse) singular matrix, can't invert!" << endl;
        det = 1;
    }
    result = result.transpose() * (1.0f/det);
    return result;
}

Matrix4 Matrix4::adjoint() const {
    Matrix4 result;
    float a1, a2, a3, a4, b1, b2, b3, b4;
    float c1, c2, c3, c4, d1, d2, d3, d4;

    a1 = m[0][0]; b1 = m[0][1];
    c1 = m[0][2]; d1 = m[0][3];

    a2 = m[1][0]; b2 = m[1][1];
    c2 = m[1][2]; d2 = m[1][3];

    a3 = m[2][0]; b3 = m[2][1];
    c3 = m[2][2]; d3 = m[2][3];

    a4 = m[3][0]; b4 = m[3][1];
    c4 = m[3][2]; d4 = m[3][3];

    result.set(
        det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4),
        - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4),
        det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4),
        - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4),

        - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4),
        det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4),
        - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4),
        det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4),

        det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4),
        - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4),
        det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4),
        - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4),

        - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3),
        det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3),
        - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3),
        det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3));
    return result;
}

float Matrix4::determinant() const {
    float a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

    a1 = m[0][0]; b1 = m[0][1]; c1 = m[0][2]; d1 = m[0][3];
    a2 = m[1][0]; b2 = m[1][1]; c2 = m[1][2]; d2 = m[1][3];
    a3 = m[2][0]; b3 = m[2][1]; c3 = m[2][2]; d3 = m[2][3];
    a4 = m[3][0]; b4 = m[3][1]; c4 = m[3][2]; d4 = m[3][3];

    return
        a1 * det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4)
        - b1 * det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4)
        + c1 * det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4)
        - d1 * det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
}

//
// Modifying operations.
//

Matrix4& Matrix4::operator=(const Matrix4& other) {
    copyFrom(other);
    return *this;
}

Matrix4& Matrix4::operator+=(const Matrix4& other) {
    this->plus(other, *this);
    return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& other) {
    this->minus(other, *this);
    return *this;
}

Matrix4& Matrix4::operator*=(float amount) {
    this->times(amount, *this);
    return *this;
}

Matrix4& Matrix4::operator/=(float amount) {
    assert(amount != 0);
    this->times(1.0f/amount, *this);
    return *this;
}

//
// Binary operations.
//

Matrix4 Matrix4::operator+(const Matrix4& other) const {
    Matrix4 result;
    this->plus(other, result);
    return result;
}

Matrix4 Matrix4::operator-(const Matrix4& other) const {
    Matrix4 result;
    this->minus(other, result);
    return result;
}

Matrix4 Matrix4::operator*(float factor) const {
    Matrix4 result;
    this->times(factor, result);
    return result;
}

Matrix4 Matrix4::operator/(float factor) const {
    assert(factor != 0);
    Matrix4 result;
    this->times(1.0f/factor, result);
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;
    this->times(other, result);
    return result;
}

Float4 Matrix4::operator*(const Float4& other) const {
    Float4 result;
    this->times(other, result);
    return result;
}


//
// Output to a stream, in tidy format
//
ostream& operator<<(ostream& os, const Matrix4& mat) {
    out_delimited(os, mat[0], '/', '\\');
    os << "\n";
    out_delimited(os, mat[1], '|', '|');
    os << "\n";
    out_delimited(os, mat[2], '|', '|');
    os << "\n";
    out_delimited(os, mat[3], '\\', '/');
    os << "\n";
    return os;
}

//
// Read 16 numbers from a stream
//
istream& operator>>(istream& is, Matrix4& m) {
    for (int row=0; row<4; row++)
        for (int col=0; col<4; col++)
        {
            float val;
            is >> val;
            m[row][col] = val;
        }
    return is;
}

//
// Copy all components of "other" into this point.
//
void Float4::copyFrom(const Float4& other) {
    for (int i = 0; i < 4; i++)
        v[i] = other.v[i];
}

//
// Add all four components of this and "other" into sum.
//
void Float4::plus(const Float4& other, Float4& sum) const {
    for (int i = 0; i < 4; i++)
        sum.v[i] = v[i] + other.v[i];
}

//
// Subtract all four components of "other" from this point, into difference
//
void Float4::minus(const Float4& other, Float4& difference) const {
    for (int i = 0; i < 4; i++)
        difference.v[i] = v[i] - other.v[i];
}

//
// Dot product of all four (X Y Z W) components of "other" and this point.
//
float Float4::dot(const Float4& other) const {
    double sum = 0;
    for (int i = 0; i < 4; i++)
        sum += v[i] * other.v[i];
    return sum;
}

//
// Divide all 4 of this point components by W (if W==0, do nothing),
// into result.
//
void Float4::homogenize(Float4& result) const {
    double w = v[3];
    if (w != 0) {
        for (int i = 0; i < 4; i++)
            result[i] /= w;
    }
}

//
// Multiply all four (X Y Z W) components of this point by scale factor,
// into result.
//
void Float4::times(float factor, Float4& result) const {
    for (int i = 0; i < 4; i++)
        result.v[i] = factor * v[i];
}


//
// return the distance from this point to "other".
//
float Point4::distanceTo(Point4& other) const {
    Vector4 offset = *this - other;
    return offset.length();
}

//
// Length, using three (X Y Z) components of this point.
//
float Vector4::length() const {
    return sqrt(*this * *this);
}

//
// Divide first three (X Y Z) components of this point by length,
// into result (if length==0, do nothing)
//
void Vector4::normalize(Vector4& result) const {
    double l = this->length();
    if (l != 0) {
        times(1/l, result);
    }
}

//
// Cross product of this vector x "other", into result
// Use only X Y Z components.
// Sets W component of product to 0.
//
void Vector4::cross(const Vector4& other, Vector4& result) const {
    result.v[0] = v[1] * other.v[2] - v[2] * other.v[1];
    result.v[1] = v[2] * other.v[0] - v[0] * other.v[2];
    result.v[2] = v[0] * other.v[1] - v[1] * other.v[0];
}

//
// return the angle (in radians) between this vector and "other".
// If this or other is (0 0 0), return 0 angle.
//
float Vector4::angle(Vector4& other) const {
    double cosine = (*this * other) / (length() * other.length());
    return acos(cosine);
}

//
// Copy all components of other matrix into this.
//
void Matrix4::copyFrom(const Matrix4& other) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            m[row][col] = other.m[row][col];
        }
    }
}

//
// Add all components of this and "other" matrix into sum
//
void Matrix4::plus(const Matrix4& other, Matrix4& sum) const {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            sum.m[row][col] = m[row][col] + other.m[row][col];
        }
    }
}

//
// Subtract all components of this matrix minus "other", into difference
//
void Matrix4::minus(const Matrix4& other, Matrix4& difference) const {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            difference.m[row][col] = m[row][col] - other.m[row][col];
        }
    }
}

//
// Multiply all components by factor, into product
//
void Matrix4::times(float factor, Matrix4& product) const {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            product.m[row][col] = m[row][col] * factor;
        }
    }
}

//
// Multiply (*this) x ("other"), and put product into product
//
// CAREFULL!! this may == &product!  So watch for partially-modified entries.
// Store result in temp array first!
//
void Matrix4::times(const Matrix4& other, Matrix4 &product) const {
    Matrix4 result;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            double sum = 0;
            for (int j = 0; j < 4; j++)
                sum += m[row][j] * other.m[j][col];
            result.m[row][col] = sum;
        }
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            product.m[row][col] = result.m[row][col];
        }
    }
}

//
// Multiply (*this) x ("point"), and put resulting point into "product".
// CAREFULL!! point may == product, so watch for partially-modified entries.
// Store result in temp array first!
//
void Matrix4::times(const Float4& point, Float4& product) const {
    Float4 result;
    for (int row = 0; row < 4; row++) {
        double sum = 0;
        for (int j = 0; j < 4; j++)
            sum += m[row][j] * point[j];
        result[row] = sum;
    }

    for (int row = 0; row < 4; row++) {
        product[row] = result[row];
    }
}

//
// Transpose this matrix, put result into result.
//
// CAREFULL!! possibly this == &result!  Watch for partially-modified entries.
// Store result in temp array first!
//
void Matrix4::transpose(Matrix4& result) const {
    Matrix4 temp;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            temp.m[row][col] = m[col][row];
        }
    }

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result.m[row][col] = temp.m[row][col];
        }
    }

}

//
// Set this matrix to identity matrix.
//
void Matrix4::setToIdentity() {
    set(1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1);
}

//
// Set this matrix to rotation about X axis.
// "angle" is in DEGREES.
//
void Matrix4::setToXRotation(float angle) {
    double rads = angle * 3.1416 / 180;
    double s = sin(rads);
    double c = cos(rads);
    set(1,0,0,0,
        0,c,-s,0,
        0,s,c,0,
        0,0,0,1);
}

//
// Set this matrix to rotation about Y axis.
// "angle" is in DEGREES.
//
void Matrix4::setToYRotation(float angle) {
    double rads = angle * 3.1416 / 180;
    double s = sin(rads);
    double c = cos(rads);
    set(c,0,s,0,
        0,1,0,0,
        -s,0,c,0,
        0,0,0,1);
}

//
// Set this matrix to rotation about Z axis.
// "angle" is in DEGREES.
//
void Matrix4::setToZRotation(float angle) {
    double rads = angle * 3.1416 / 180;
    double s = sin(rads);
    double c = cos(rads);
    set(c,-s,0,0,
        s,c,0,0,
        0,0,1,0,
        0,0,0,1);
}

//
// Set this matrix to translation matrix.
//
void Matrix4::setToTranslation(float tx, float ty, float tz) {
    set(1,0,0,tx,
        0,1,0,ty,
        0,0,1,tz,
        0,0,0,1);
}

//
// Set this matrix to scaling matrix.
//
void Matrix4::setToScaling(float sx, float sy, float sz) {
    set(sx,0,0,0,
        0,sy,0,0,
        0,0,sz,0,
        0,0,0,1);
}

//
// Default constructor
//
Plane4::Plane4() {
    set(0,0,0,1);
}

// construct the of the plane passing through
// pointOnPlane and orthogonal to normal.
Plane4::Plane4(const Point4& pointOnPlane, const Vector4& normal)
{
    Q = pointOnPlane;
    N = normal;
    set(N.X(), N.Y(), N.Z(), -N.dot(Q));
}

//
// evaluates this implicit plane equation at "other".
// IE, evaluates N dot (p - Q)
//
float Plane4::at(Point4& p) const
{
//    return N * (p - Q);
    return dot(p);
}

// returns true iff this plane is parallel to "other".
bool Plane4::isParallelTo(Plane4& other) const
{
    return N.angle(other.N) < EPSILON;
}

ostream& operator<<(ostream& os, const Plane4& p) {
    os << "Through " << p.Q << " normal to " << p.N;
    return os;
}

//
// Default constructor
//
Ray4::Ray4() {
    start.set(0,0,0);
    direction.set(1,0,0);
}

//
// Constructor, given a start and a direction.
//
Ray4::Ray4(Point4& s, Vector4& d)
{
    start = s;
    direction = d;
}

//
// Constructor, given a start point and target point
//
Ray4::Ray4(Point4& s, Point4& target)
{
    start = s;
    direction = target - s;
}

//
// The point which is t units of "direction" forward from "start"
// (ie, start + t * direction)
Point4 Ray4::at(float t) const
{
    return start + t * direction;
}

//
// Computes intersection between this ray and plane
// If ray hits plane, sets the hitpoint and returns true.
// If ray misses, returns false.
// NOTE.  Intersections must have a POSITIVE t!
bool Ray4::intersects(const Plane4& plane, Point4& hitPoint) const
{
    float num   = plane.N * (plane.Q - start);
    float denom = plane.N * direction;
    if (fabs(denom) > EPSILON)
    {
        float t = num / denom;
        hitPoint = at(t);
        return true;
    }
    else
    {
        return false;
    }
}

ostream& operator<<(ostream& os, const Ray4& r) {
    os << "S: " << r.start << " V: " << r.direction;
    return os;
}

