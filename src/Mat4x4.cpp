#include "Mat4x4.h"





float Vec4::operator[](unsigned int iColumn)
{
  if(iColumn >= 4)
    return 0;
  return data[iColumn];
}


Vec4& Vec4::operator*(const Mat4x4& m)
{
  return
  {
      data[0] * m[0][0] + data[1] * m[1][0] + data[2] * m[2][0] + data[3] * m[3][0],
      data[0] * m[0][1] + data[1] * m[1][1] + data[2] * m[2][1] + data[3] * m[3][1],
      data[0] * m[0][2] + data[1] * m[1][2] + data[2] * m[2][2] + data[3] * m[3][2],
      data[0] * m[0][3] + data[1] * m[1][3] + data[2] * m[2][3] + data[3] * m[3][3]
  }
}



Vec4& Mat4x4::operator[](unsigned int iRow)
{
  if(iRow >= 4)
    return {0.0f,0.0f,0.0f,0.0f};
  return {data[iRow]};
}




Mat4x4 Mat4x4::operator*(const Mat4x4& m)
{

}


Mat4x4 Mat4x4::operator+(const Mat4x4 m)
{

}


Mat4x4 Mat4x4::operator-(const Mat4x4 m)
{

}
