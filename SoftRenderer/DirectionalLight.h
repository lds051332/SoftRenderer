#ifndef DIRECTIONALLIGHT_H
#define  DIRECTIONALLIGHT_H

#include "Light.h"

//ȫ��ֱ���
class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(Vector4 pos, Vector4 trg, Color clr, float indst);
	~DirectionalLight();

	Vector4 TargetPos;
	
	//�����ֱ�������������ϵ�еķ�������
	Vector4 DirectInWorld();

private:

};

#endif