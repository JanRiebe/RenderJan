
/*
Points are treated as row vectors.

*/

// Forward declarations:
class Vec4;





class Mat4x4
{
private:
  float data[4][4];    // Row, Column

public:

  Vec4 operator[](unsigned int iRow);

  Vec4& operator*(const Mat4x4& m);
  Mat4x4& operator+(const Mat4x4& m);
  Mat4x4& operator-(const Mat4x4& m);

};



struct Vec4
{
private:
  float data[4];

public:
  float operator[](unsigned int iColumn);
  Vec4& operator*(const Mat4x4& m);
};
