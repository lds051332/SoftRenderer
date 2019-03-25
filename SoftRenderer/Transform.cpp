#include "Transform.h"

Transform::Transform()
{
	Position = Vector4(0, 0, 0, 1);
	Rotation = Vector4(0, 0, 0, 1);
	Scale = Vector4(1, 1, 1, 1);
}

Transform::Transform(Vector4 p, Vector4 r, Vector4 s)
{
	Position = p;
	Rotation = r;
	Scale = s;
}

Transform::~Transform()
{
}