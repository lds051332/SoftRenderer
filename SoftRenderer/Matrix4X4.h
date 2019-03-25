#ifndef  MATRIX4X4_H
#define  MATRIX4X4_H
#include "Vector4.h"
class Vector4;
class Matrix4X4
{
public:
	Matrix4X4();
	Matrix4X4(float mc[4][4]);
	~Matrix4X4();
	float m[4][4];
	//����˷�
	//static void Matrix_Mul(Matrix4X4* pOut, const Matrix4X4 *a, const Matrix4X4 *b);
	//�����˾���
	static void Vec_Mat_Mul(Vector4* pOut, const Vector4* vec, const Matrix4X4* mat);
	//�������
	//static void Matrix_Set_Zero(Matrix4X4* m);
	//��Ϊ��λ����
	//static void Matrix_Set_Identity(Matrix4X4 *m);

	void Set_Identity();
	void Set_Zero();

	//���������
	Matrix4X4 operator* (const Matrix4X4& b);
private:
	
};

#endif