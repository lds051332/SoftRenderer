#include "Vertex.h"

Vertex::Vertex()
{
	color = Color(1.0f, 1.0f, 1.0f, 1);
	point = Vector4(0, 0, 0, 1);
	normal = Vector4(0, 0, 0, 0);;
	uv1 = Vector2(0,0);
	uv2 = Vector2(0, 0);
	rhw = 1.0f;
}

Vertex::~Vertex()
{
}