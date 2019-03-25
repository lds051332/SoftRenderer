#ifndef DIRECTIONALLIGHT_H
#define  DIRECTIONALLIGHT_H

#include "Light.h"

//全局直射光
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(Vector4 pos, Vector4 trg, Color clr, float indst);
	~DirectionalLight();

	Vector4 TargetPos;
	
	//计算出直射光在世界坐标系中的方向向量
	Vector4 DirectInWorld();

private:

};

#endif