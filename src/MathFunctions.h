#pragma once


bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

double InverseSquareFalloff(double intensity, double distance);

float clamp(float floor, float ceil, float in);

float* MatMult(float a[16], float b[16]);
float* HomogMat(float m[16]);
float* VecMatMult(float vec[4], float mat[16]);
float* HomogVec(float vec[4]);
