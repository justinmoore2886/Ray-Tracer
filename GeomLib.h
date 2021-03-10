#ifndef _GEOMLIB_H_
#define _GEOMLIB_H_

#ifdef WIN32
#include <windows.h>
#endif

#include <assert.h>
#include <iostream>
#include <math.h>

#ifndef EPSILON
#define EPSILON 0.00001
#endif

#ifndef M_PI
#define M_PI 3.141592656358979
#endif

using namespace std;

class Point4;

class Float4 {
public:
    //
    // Copy all components of "other" into this point.
    //
    void copyFrom(const Float4& other);

    //
    // Add all four components of this and "other" into sum.
    //
    void plus(const Float4& other, Float4& sum) const;

    //
    // Subtract all four components of "other" from this point, into difference
    //
    void minus(const Float4& other, Float4& difference) const;

    //
    // Dot product of all four (X Y Z W) components of "other" and this point.
    //
    float dot(const Float4& other) const;

    //
    // Divide all 4 of this point components by W (if W==0, do nothing),
    // into result.
    //
    void homogenize(Float4& result) const;

    //
    // Multiply all four (X Y Z W) components of this point by scale factor,
    // into result.
    //
    void times(float factor, Float4& result) const;

    //
    // Default constructor
    //
    Float4();

    //
    // Explicit constructor
    //
    Float4(const float x, const float y, const float z, const float w);

    //
    // Copy constructor
    //
    Float4(const Float4 &r);

    //
    // Intialize all 4 components
    //
    void set(float x, float y, float z, float w);

    Point4 homogenized();

    // Compare for equality

    bool operator==(const Float4& other);

    // Access internal components of point's data array.

    // L-value (on left of = sign)

    float& operator [](int index);

    // R-value (on right of = sign)

    const float& operator [](int index) const;

    // Named access to the four components: L-values

    float& X();
    float& Y();
    float& Z();
    float& W();

    // Named access to the components: R-values

    const float& X() const;
    const float& Y() const;
    const float& Z() const;
    const float& W() const;

    //
    // Output to stream
    //

    friend ostream& operator<<(ostream& os,const Float4& m);

    friend void out_delimited(ostream& os, const Float4& m,
                              char l_delim, char r_delim);

    // Read from stream

    friend istream& operator>>(istream& is, Float4& p);

protected:

    // The data is stored here.

    float v[4];
};

class Vector4;

class Point4 : public Float4 {
public:
    //
    // return the distance from this point to "other".
    //
    float distanceTo(Point4& other) const;

    //
    // Default constructor
    //
    Point4();

    //
    // Explicit constructor
    //
    Point4(const float x, const float y, const float z);

    //
    // Change the values
    //
    void set(float x, float y, float z);

    // Assignment.  Checks W component, just in case
    Point4& operator=(const Float4& r);

    // Add a vector to this point
    Point4& operator+=(const Vector4& r);

    // Binary arithmetic operations

    // Sum of this point and a vector
    Point4 operator+(const Vector4& other) const;

    // Subtract a vector from this point
    Point4 operator-(const Vector4& other) const;

    // Difference of two points is a vector
    Vector4 operator-(const Point4& other) const;
};

class Vector4 : public Float4 {
public:
    //
    // Length, using three (X Y Z) components of this point.
    //
    float length() const;

    //
    // Divide first three (X Y Z) components of this point by length,
    // into result (if length==0, do nothing)
    //
    void normalize(Vector4& result) const;

    //
    // Cross product of this vector x "other", into result
    // Use only X Y Z components.
    // Sets W component of product to 0.
    //
    void cross(const Vector4& other, Vector4& result) const;

    //
    // return the angle (in radians) between this vector and "other".
    //
    float angle(Vector4& other) const;

    //
    // Default constructor
    //
    Vector4();

    //
    // Explicit constructor
    //
    Vector4(float x, float y, float z);

    void set(float x, float y, float z);

//    operator const Float4 () const;

//    operator const Float4& () const;

    // Assignment.  Checks W component, just in case
    Vector4& operator=(const Float4& r);

    // subtract a vector from this
    Vector4& operator-=(const Vector4& r);

    // add a vector to this
    Vector4& operator+=(const Vector4& r);

    // scale this vector
    Vector4& operator*=(float amount);

    // scale this vector by reciprocal
    Vector4& operator/=(float amount);

    // Unary arithmetic operations.

    // Negate (flip) this vector
    Vector4 operator-() const;

    // Binary arithmetic operations

    // Add two vectors
    Vector4 operator+(const Vector4& other) const;

    // subtract two vectors
    Vector4 operator-(const Vector4& other) const;

    // add a point: yields a point
    Point4 operator+(const Point4& other) const;

    // Vector4-and-scalar arithmetic operations
    //
    // vector * scalar

    Vector4 operator*(float factor) const;

    //
    // scalar * vector
    //

    friend Vector4 operator*(float factor, const Vector4& other);

    // divide (scale by reciprocal)
    Vector4 operator/(float factor) const;

    //
    // Returns a normalized version of this vector.
    //
    Vector4 normalized() const;

    //
    // Returns dot product of this and other.
    // * operator is DOT product !!!!!
    //
    float operator*(const Vector4& other) const;

    //
    // Returns cross product of this and other.
    // ^ operator is CROSS product !!!!
    //
    Vector4 operator^(const Vector4& other) const;
};

class Matrix4 {
public:
    //
    // Default constructor: set to identity matrix.
    //
    Matrix4();

    //
    // Explicit constructor.
    //

  Matrix4(float xx, float xy, float xz, float xw,
          float yx, float yy, float yz, float yw,
          float zx, float zy, float zz, float zw,
          float wx, float wy, float wz, float ww);

  //
  // Initialize all 16 components.
  //

  void set(float xx, float xy, float xz, float xw,
           float yx, float yy, float yz, float yw,
           float zx, float zy, float zz, float zw,
           float wx, float wy, float wz, float ww);

  //
  // Copy Constructor.
  //

  Matrix4(const Matrix4& other);

  //
  // Set a single entry.
  //
  void set(int row, int col, float value);

  //
  // Copy all components of other matrix into this.
  //
  void copyFrom(const Matrix4& other);

  //
  // Add all components of this and "other" matrix into sum
  //
  void plus(const Matrix4& other, Matrix4& sum) const;

  //
  // Subtract all components of this matrix minus "other", into difference
  //
  void minus(const Matrix4& other, Matrix4& difference) const;

  //
  // Multiply all components by factor, into product
  //
  void times(float factor, Matrix4& product) const;

  //
  // Multiply (*this) x ("other"), and put product into product
  //
  // CAREFULL!! this may = &product!  So watch for partially-modified entries.
  //
  void times(const Matrix4& other, Matrix4 &product) const;

  //
  // Multiply (*this) x ("point"), and put resulting point into "product".
  //
  void times(const Float4& point, Float4& product) const;

  //
  // Transpose this matrix, put result into result.
  //
  // CAREFULL!! this may = &result!  So watch for partially-modified entries.
  //
  void transpose(Matrix4& result) const;

  //
  // Set this matrix to identity matrix.
  //
  void setToIdentity();

  //
  // Set this matrix to rotation about X axis.
  // "angle" is in DEGREES.
  //
  void setToXRotation(float angle);

  //
  // Set this matrix to rotation about Y axis.
  // "angle" is in DEGREES.
  //
  void setToYRotation(float angle);

  //
  // Set this matrix to rotation about Z axis.
  // "angle" is in DEGREES.
  //
  void setToZRotation(float angle);

  //
  // Set this matrix to translation matrix.
  //
  void setToTranslation(float tx, float ty, float tz);

  //
  // Set this matrix to scaling matrix.
  //
  void setToScaling(float sx, float sy, float sz);

  //
  // Array-access operators
  //

  Float4& operator[](int index);

  const Float4& operator [](int index) const;

  //
  // Unary operations
  //

  Matrix4 transpose() const;

  Matrix4 adjoint() const;

  Matrix4 inverse() const;

  float determinant() const;

  Matrix4 operator-() const;


  //
  // Static methods for determinants
  //

  static float det3x3(float a1,float a2,float a3,
                      float b1,float b2,float b3,
                      float c1,float c2,float c3) {
      return
            a1 * det2x2( b2, b3, c2, c3 )
          - b1 * det2x2( a2, a3, c2, c3 )
          + c1 * det2x2( a2, a3, b2, b3 );
  };

  static float det2x2(float a, float b,
                      float c, float d) {
      return a * d - b * c;
  };

  //
  // Modifying operations.
  //

  Matrix4& operator=(const Matrix4& other);

  Matrix4& operator+=(const Matrix4& other);

  Matrix4& operator-=(const Matrix4& other);

  Matrix4& operator*=(float amount);

  Matrix4& operator/=(float amount);

  //
  // Binary operations.
  //

  Matrix4 operator+(const Matrix4& other) const;

  Matrix4 operator-(const Matrix4& other) const;

  Matrix4 operator*(float factor) const;

  Matrix4 operator/(float factor) const;

  Matrix4 operator*(const Matrix4& other) const;

  Float4 operator*(const Float4& other) const;

  //
  // Static matrices: standard transformations
  //

  static Matrix4 Identity() {
      Matrix4 result;
      result.setToIdentity();
      return result;
  }

  static Matrix4 XRotation(float angle) {
      Matrix4 result;
      result.setToXRotation(angle);
      return result;
  }

  static Matrix4 YRotation(float angle) {
      Matrix4 result;
      result.setToYRotation(angle);
      return result;
  }

  static Matrix4 ZRotation(float angle) {
      Matrix4 result;
      result.setToZRotation(angle);
      return result;
  }

  static Matrix4 Translation(float tx, float ty, float tz) {
      Matrix4 result;
      result.setToTranslation(tx,ty,tz);
      return result;
  }

  static Matrix4 Scaling(float sx, float sy, float sz) {
      Matrix4 result;
      result.setToScaling(sx,sy,sz);
      return result;
  }

  //
  // Output to a stream, in tidy format
  //
  friend ostream& operator<<(ostream& os, const Matrix4& mat);

  //
  // Read 16 numbers from a stream
  //
  friend istream& operator>>(istream& is, Matrix4& m);

private:
  //
  // The matrix itself.
  //
  Float4 m[4];
};

class Plane4 : public Float4 {
public:
    // evaluates *this implicit plane equation at "other".
    float at(Point4& other) const;

    // returns true iff *this is parallel to "other".
    bool isParallelTo(Plane4& other) const;

    //
    // Default constructor
    //
    Plane4();

    // construct the of the plane passing through
    // pointOnPlane and orthogonal to normal.
    Plane4(const Point4& pointOnPlane, const Vector4& normal);

    Point4 Q;  // A point on the plane
    Vector4 N; // A vector normal to the plane

    friend ostream& operator<<(ostream& os,const Plane4& p);

};

class Ray4 {
public:
    //
    // The point which is t units of "direction" forward from "start"
    // (ie, start + t * direction)
    Point4 at(float t) const;

    //
    // Computes intersection between this ray and plane
    // If ray hits plane, sets the hitpoint and returns true.
    // If ray misses, returns false.
    // NOTE.  Intersections must have a POSITIVE t!
    bool intersects(const Plane4& plane, Point4& hitPoint) const;

    //
    // Default constructor
    //
    Ray4();

    //
    // Constructor, given a start and a direction.
    //
    Ray4(Point4& s, Vector4& d);

    //
    // Constructor, given a start point and target point
    //
    Ray4(Point4& s, Point4& target);

    Point4 start;
    Vector4 direction;

    friend ostream& operator<<(ostream& os,const Ray4& r);

};

#endif
