#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	enabled = true;
	color = WHITE;
	intensity = 1.0f;
	position = Vector4(1, 1, 1, 0);
	TargetPos = Vector4(0, 0, 0, 0);
}

DirectionalLight::DirectionalLight(Vector4 pos, Vector4 trg, Color clr, float indst)
{
	enabled = true;
	color = clr;
	intensity = indst;
	position = pos;
	TargetPos = trg;
}

DirectionalLight::~DirectionalLight()
{
}

Vector4 DirectionalLight::DirectInWorld()
{
	Vector4 re;
	re = position - TargetPos;
	re.Normalize();
	return re;
}