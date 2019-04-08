#pragma once


bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

double InverseSquareFalloff(double intensity, double distance);

float clamp(float floor, float ceil, float in);


struct Matrix4x4
{
  float values[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,};

  float& operator[](int i)
  {
    return values[i%16];
  }
};



Matrix4x4 MatMult(Matrix4x4 a, Matrix4x4 b);
Matrix4x4 HomogMat(Matrix4x4 m);
float* VecMatMult(float vec[4], Matrix4x4 mat);
float* HomogVec(float vec[4]);

Matrix4x4 GetRotationMatrix(float angleX, float angleY, float angleZ);
