#ifndef TRANSFORM_H
#define  TRANSFORM_H

#include "Vector4.h"
#include "Matrix4X4.h"

class Transform
{
public:
	Transform();
	Transform(Vector4 p, Vector4 r, Vector4 s);
	~Transform();

	Vector4 Position;
	Vector4 Rotation;
	Vector4 Scale;

private:

};



#endif