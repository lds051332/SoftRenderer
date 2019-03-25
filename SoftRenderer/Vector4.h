#ifndef  VECTOR4_H
#define  VECTOR4_H
#include "Matrix4X4.h"
class Matrix4X4;
class Vector4
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	~Vector4();
	float x;
	float y;
	float z;
	float w;
	//�������
	static void Vec_Cross(Vector4* pOut, const Vector4* a, const Vector4* b);
	//�������
	static float Dot(const Vector4* a, const Vector4* b);
	//�������
	static void Vec_Plus(Vector4* pOut, const Vector4* a, const Vector4* b);
	//�������
	static void Vec_Minus(Vector4* pOut, const Vector4* a, const Vector4* b);
	//������ģ
	//static float Vec_Length(const Vector4* a);
	//�����淶��
	//static void Vec_Normalize(Vector4* a);
	//W��һ��  ͸�ӳ���
	//static void Vec_Homogenize(Vector4* pOut);

	void Normalize();
	void Homogenize();
	float Length();

	//���������
	void operator= (const Vector4& b);
	Vector4 operator* (const Vector4& b);
	Vector4 operator+ (const Vector4& b);
	Vector4 operator- (const Vector4& b);
	Vector4 operator* (const Matrix4X4& b);
	Vector4 operator* (const float& b);
private:

};
#endif

