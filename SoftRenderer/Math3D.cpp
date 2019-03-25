#include "Math3D.h"
#include <math.h>

Vector4 Math3D::Vec_Mat_Mul(const Vector4* vec, const Matrix4X4* mat)
{
	float rx, ry, rz, rw;
	rx = vec->x*mat->m[0][0] + vec->y*mat->m[1][0] + vec->z*mat->m[2][0] + vec->w*mat->m[3][0];
	ry = vec->x*mat->m[0][1] + vec->y*mat->m[1][1] + vec->z*mat->m[2][1] + vec->w*mat->m[3][1];
	rz = vec->x*mat->m[0][2] + vec->y*mat->m[1][2] + vec->z*mat->m[2][2] + vec->w*mat->m[3][2];
	rw = vec->x*mat->m[0][3] + vec->y*mat->m[1][3] + vec->z*mat->m[2][3] + vec->w*mat->m[3][3];
	return Vector4(rx, ry, rz, rw);
}

Matrix4X4 Math3D::Translate(float x, float y, float z)
{
	Matrix4X4 m;
	m.Set_Identity();
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
	return m;
}

Matrix4X4 Math3D::SetScale(float x, float y, float z)
{
	Matrix4X4 m;
	m.Set_Identity();
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
	return m;
}

Matrix4X4 Math3D::RotateX(float rotateAngle)
{
	rotateAngle = PI / 180.0f * rotateAngle;
	Matrix4X4 m;
	m.Set_Identity();
	m.m[1][1] = cos(rotateAngle);
	m.m[1][2] = sin(rotateAngle);
	m.m[2][1] = -sin(rotateAngle);
	m.m[2][2] = cos(rotateAngle);
	return m;
}

Matrix4X4 Math3D::RotateY(float rotateAngle)
{
	rotateAngle = PI / 180.0f * rotateAngle;
	Matrix4X4 m;
	m.Set_Identity();
	m.m[0][0] = cos(rotateAngle);
	m.m[0][2] = -sin(rotateAngle);
	m.m[2][0] = sin(rotateAngle);
	m.m[2][2] = cos(rotateAngle);
	return m;
}

Matrix4X4 Math3D::RotateZ(float rotateAngle)
{
	rotateAngle = PI / 180.0f * rotateAngle;
	Matrix4X4 m;
	m.Set_Identity();
	m.m[0][0] = cos(rotateAngle);
	m.m[0][1] = sin(rotateAngle);
	m.m[1][0] = -sin(rotateAngle);
	m.m[1][1] = cos(rotateAngle);
	return m;
}

float Math3D::Lerp(float a, float b, float t)
{
	if (t <= 0.0)
	{
		return a;
	}
	else if (t >= 1.0)
	{
		return b;
	}
	else
	{
		return b * t + (1.0 - t) * a;
	}
}

Vector4 Math3D::Lerp(Vector4 a, Vector4 b, float t)
{
	Vector4 re;
	re.x = Lerp(a.x, b.x, t);
	re.y = Lerp(a.y, b.y, t);
	re.z = Lerp(a.z, b.z, t);
	re.w = Lerp(a.w, b.w, t);
	return re;
}

Color Math3D::Lerp(Color a, Color b, float t)
{
	Color re;
	re.r = Lerp(a.r, b.r, t);
	re.g = Lerp(a.g, b.g, t);
	re.b = Lerp(a.b, b.b, t);
	re.a = Lerp(a.a, b.a, t);
	return re;
}

void Math3D::Clamp(float& x, const float min, const float max)
{
	if (x < min)
	{
		x = min;
	}
	else if (x > max)
	{
		x = max;
	}
}

void Math3D::Clamp(int& x, const int min, const int max)
{
	if (x < min)
	{
		x = min;
	}
	else if (x > max)
	{
		x = max;
	}
}

void Math3D::Swap(int &x, int &y)
{
	int temp = y;
	y = x;
	x = temp;
}