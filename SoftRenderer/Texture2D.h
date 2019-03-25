#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Color.h"
#include <iostream>
#include <windows.h>
#include <string>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();
	//COLORREF** textureData;
	Color** pixelColorData;
	int texWidth;
	int texHeight;

	//根据图片长宽，初始化COLORREF的二维数组

	//读取BMP
	bool LoadBmp(std::string bmpName);
private:
	bool Init();

};


#endif