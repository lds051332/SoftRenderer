#include "Matrix4X4.h"
#include "Vector4.h"
#include "Color.h"
#ifndef  MATH3D_H
#define  MATH3D_H

namespace Math3D{

	#define PI 3.1415926f

	Vector4 Vec_Mat_Mul(const Vector4* vec, const Matrix4X4* mat);

	Matrix4X4 Translate(float x, float y, float z);

	Matrix4X4 SetScale(float x, float y, float z);

	//注：之前旋转的效果不正确，是因为实现的时候使用的是弧度，传参的时候传的是角度，修改之后就正确了
	//角度 to 弧度: π / 180×角度
	//弧度 to 角度: 180 / π×弧度

	//参数为角度，不是弧度
	Matrix4X4 RotateX(float rotateAngle);
	//参数为角度，不是弧度
	Matrix4X4 RotateY(float rotateAngle);
	//参数为角度，不是弧度
	Matrix4X4 RotateZ(float rotateAngle);

	//插值
	float Lerp(float a, float b, float t);
	Vector4 Lerp(Vector4 a, Vector4 b, float t);
	Color Lerp(Color a, Color b, float t);
	//限制，float
	void Clamp(float& x, const float min, const float max);
	//限制，int
	void Clamp(int& x, const int min, const int max);
	//交换值
	void Swap(int &x, int &y);
}

#endif