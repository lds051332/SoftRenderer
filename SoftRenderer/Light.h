#ifndef LIGHT_H
#define  LIGHT_H

#include "Vector4.h"
#include "Color.h"
#include <windows.h>

//นโป๙ภเ
class Light
{
public:
	Vector4 position;

	bool enabled;
	Color color;
	float intensity;

private:

};

#endif