#ifndef  VERTEX_H
#define  VERTEX_H

#include "Vector4.h"
#include "Vector2.h"
#include "Color.h"
#include <windows.h>

class Vertex
{
public:
	Vertex();
	~Vertex();

	Color color;
	Vector4 point;
	Vector4 normal;
	Vector2 uv1;
	Vector2 uv2;
	float rhw; //reciprocal homogeneous w  wµ¹Êý

private:

};

#endif
