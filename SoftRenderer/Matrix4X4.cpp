#include "Matrix4X4.h"


Matrix4X4::Matrix4X4()
{
	Set_Identity();
}

Matrix4X4::Matrix4X4(float mp[4][4])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[j][i] = mp[j][i];
		}
	}
}

Matrix4X4::~Matrix4X4()
{
}

//æÿ’Û≥À∑®
/*void Matrix4X4::Matrix_Mul(Matrix4X4* pOut, const Matrix4X4 *a, const Matrix4X4 *b)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			pOut->m[j][i] = (a->m[j][0] * b->m[0][i]) +
				(a->m[j][1] * b->m[1][i]) +
				(a->m[j][2] * b->m[2][i]) +
				(a->m[j][3] * b->m[3][i]);
		}
	}
}*/
//œÚ¡ø≥Àæÿ’Û
void Matrix4X4::Vec_Mat_Mul(Vector4* pOut, const Vector4* vec, const Matrix4X4* mat)
{
	pOut->x = vec->x*mat->m[0][0] + vec->y*mat->m[1][0] + vec->z*mat->m[2][0] + vec->w*mat->m[3][0];
	pOut->y = vec->x*mat->m[0][1] + vec->y*mat->m[1][1] + vec->z*mat->m[2][1] + vec->w*mat->m[3][1];
	pOut->z = vec->x*mat->m[0][2] + vec->y*mat->m[1][2] + vec->z*mat->m[2][2] + vec->w*mat->m[3][2];
	pOut->w = vec->x*mat->m[0][3] + vec->y*mat->m[1][3] + vec->z*mat->m[2][3] + vec->w*mat->m[3][3];
}

//æÿ’ÛπÈ¡„
/*void Matrix4X4::Matrix_Set_Zero(Matrix4X4* m)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m->m[j][i] = 0.0f;
		}
	}
}
//…ËŒ™µ•Œªæÿ’Û
void Matrix4X4::Matrix_Set_Identity(Matrix4X4 *m)
{
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}*/

void Matrix4X4::Set_Identity()
{
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	m[0][1] = m[0][2] = m[0][3] = 0.0f;
	m[1][0] = m[1][2] = m[1][3] = 0.0f;
	m[2][0] = m[2][1] = m[2][3] = 0.0f;
	m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

void Matrix4X4::Set_Zero()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[j][i] = 0.0f;
		}
	}
}
Matrix4X4 Matrix4X4::operator* (const Matrix4X4& b)
{
	float rm[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			rm[j][i] = (m[j][0] * b.m[0][i]) +
				(m[j][1] * b.m[1][i]) +
				(m[j][2] * b.m[2][i]) +
				(m[j][3] * b.m[3][i]);
		}
	}
	return Matrix4X4(rm);
}