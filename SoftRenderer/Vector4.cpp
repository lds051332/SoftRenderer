#include "Vector4.h"
#include <math.h>

Vector4::Vector4()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Vector4::Vector4(float xV, float yV, float zV, float wV)
{
	x = xV;
	y = yV;
	z = zV;
	w = wV;
}

Vector4::~Vector4()
{
}

//向量叉乘
void Vector4::Vec_Cross(Vector4* pOut, const Vector4* a, const Vector4* b)
{
	pOut->x = a->y*b->z - b->y*a->z;
	pOut->y = a->z*b->x - b->z*a->x;
	pOut->z = a->y*b->x - b->y*a->x;
	pOut->w = 1.0f;
}
//向量点乘
float Vector4::Dot(const Vector4* a, const Vector4* b)
{
	float result;
	result = a->x * b->x + a->y*b->y + a->z*b->z;
	return result;
}
//向量相加
void Vector4::Vec_Plus(Vector4* pOut, const Vector4* a, const Vector4* b)
{
	pOut->x = a->x + b->x;
	pOut->y = a->y + b->y;
	pOut->z = a->z + b->z;
	pOut->w = 1.0f;
}
//向量相减
void Vector4::Vec_Minus(Vector4* pOut, const Vector4* a, const Vector4* b)
{
	pOut->x = a->x - b->x;
	pOut->y = a->y - b->y;
	pOut->z = a->z - b->z;
	pOut->w = 1.0f;
}
//向量的模
/*float Vector4::Vec_Length(const Vector4* a)
{
	float result;
	result = sqrtf(a->x*a->x + a->y*a->y + a->z*a->z);
	return result;
}
//向量规范化
void Vector4::Vec_Normalize(Vector4* a)
{
	float length = Vec_Length(a);
	a->x = a->x / length;
	a->y = a->y / length;
	a->z = a->z / length;
	a->w = 1.0f;
}

//向量归一化  透视除法
void Vector4::Vec_Homogenize(Vector4* pOut)
{
	pOut->x = pOut->x / pOut->w;
	pOut->y = pOut->y / pOut->w;
	pOut->z = pOut->z / pOut->w;
	pOut->w = 1.0;
}*/

//向量的模
float Vector4::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

//向量规范化
void Vector4::Normalize()
{
	float length = Length();
	x = x / length;
	y = y / length;
	z = z / length;
	w = 1.0f;
}

//W归一化  透视除法
void Vector4::Homogenize()
{
	x = x / w;
	y = y / w;
	z = z / w;
	w = 1.0;
}

void Vector4::operator= (const Vector4& b)
{
	x = b.x;
	y = b.y;
	z = b.z;
	w = b.w;
}

//2019年补充
//向量叉乘
// |i  j  k | 直角坐标系（i,j,k）
// |x  y  z | 这一行就是*号左边的向量
// |bx by bz| 这一行就是*号右边的向量
//叉乘就是求行列式的值
Vector4 Vector4::operator* (const Vector4& b)
{
	float rx, ry, rz, rw;
	rx = y*b.z - b.y*z;
	ry = z*b.x - b.z*x;
	rz = x*b.y - b.x*y;
	rw = 0.0f;
	return Vector4(rx,ry,rz,rw);
}

Vector4 Vector4::operator+ (const Vector4& b)
{
	float rx, ry, rz, rw;
	rx = x + b.x;
	ry = y + b.y;
	rz = z + b.z;
	rw = 0.0f;
	return Vector4(rx, ry, rz, rw);
}

Vector4 Vector4::operator- (const Vector4& b)
{
	float rx, ry, rz, rw;
	rx = x - b.x;
	ry = y - b.y;
	rz = z - b.z;
	rw = 0.0f;
	return Vector4(rx, ry, rz, rw);
}

Vector4 Vector4::operator* (const Matrix4X4& mat)
{
	float rx, ry, rz, rw;
	rx = x*mat.m[0][0] + y*mat.m[1][0] + z*mat.m[2][0] + w*mat.m[3][0];
	ry = x*mat.m[0][1] + y*mat.m[1][1] + z*mat.m[2][1] + w*mat.m[3][1];
	rz = x*mat.m[0][2] + y*mat.m[1][2] + z*mat.m[2][2] + w*mat.m[3][2];
	rw = x*mat.m[0][3] + y*mat.m[1][3] + z*mat.m[2][3] + w*mat.m[3][3];
	return Vector4(rx, ry, rz, rw);
}

Vector4 Vector4::operator* (const float& b)
{
	float rx, ry, rz, rw;
	rx = x * b;
	ry = y * b;
	rz = z * b;
	rw = w;
	return Vector4(rx, ry, rz, rw);
}