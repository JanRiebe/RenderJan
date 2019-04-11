#pragma once


bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

double InverseSquareFalloff(double intensity, double distance);

float clamp(float floor, float ceil, float in);


struct Vec4
{
  float values[4] = {0,0,0,1};

  float& operator[](int i)
  {
    return values[i%4];
  }

  Vec4 operator-(Vec4 v2)
  {
  	return {values[0] - v2.values[0], values[1] - v2.values[1], values[2] - v2.values[2], 1};
  }
};


struct Matrix4x4
{
  float values[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1,};

  float& operator[](int i)
  {
    return values[i%16];
  }
};



Matrix4x4 MatMult(Matrix4x4 a, Matrix4x4 b);
Matrix4x4 HomogMat(Matrix4x4 m);
Vec4 VecMatMult(Vec4 vec, Matrix4x4 mat);
Vec4 HomogVec(Vec4 vec);

Matrix4x4 GetRotationMatrix(float angleX, float angleY, float angleZ);
