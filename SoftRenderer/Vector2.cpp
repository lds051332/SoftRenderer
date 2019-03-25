#include "Vector2.h"
#include <math.h>

Vector2::Vector2()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(int xV, int yV)
{
	x = xV;
	y = yV;
}

Vector2::Vector2(float xV, float yV)
{
	x = xV;
	y = yV;
}

Vector2::~Vector2()
{
}