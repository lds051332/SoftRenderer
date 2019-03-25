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

	//ע��֮ǰ��ת��Ч������ȷ������Ϊʵ�ֵ�ʱ��ʹ�õ��ǻ��ȣ����ε�ʱ�򴫵��ǽǶȣ��޸�֮�����ȷ��
	//�Ƕ� to ����: �� / 180���Ƕ�
	//���� to �Ƕ�: 180 / �С�����

	//����Ϊ�Ƕȣ����ǻ���
	Matrix4X4 RotateX(float rotateAngle);
	//����Ϊ�Ƕȣ����ǻ���
	Matrix4X4 RotateY(float rotateAngle);
	//����Ϊ�Ƕȣ����ǻ���
	Matrix4X4 RotateZ(float rotateAngle);

	//��ֵ
	float Lerp(float a, float b, float t);
	Vector4 Lerp(Vector4 a, Vector4 b, float t);
	Color Lerp(Color a, Color b, float t);
	//���ƣ�float
	void Clamp(float& x, const float min, const float max);
	//���ƣ�int
	void Clamp(int& x, const int min, const int max);
	//����ֵ
	void Swap(int &x, int &y);
}

#endif