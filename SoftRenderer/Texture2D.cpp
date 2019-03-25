#include "Texture2D.h"

Texture2D::Texture2D()
{
	//textureData = NULL;
	pixelColorData = NULL;
	texWidth = 0;
	texHeight = 0;
}

Texture2D::~Texture2D()
{
	//delete(textureData);
	delete(pixelColorData);
}

bool Texture2D::Init()
{
	if (texWidth > 0 && texHeight > 0)
	{
		//textureData = new COLORREF*[texWidth];
		pixelColorData = new Color*[texWidth];
		for (int i = 0; i <= texHeight; ++i)
		{
			//textureData[i] = new COLORREF[texWidth];
			pixelColorData[i] = new Color[texWidth];
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Texture2D::LoadBmp(std::string bmpName)
{
	const char* charPath = bmpName.data();
	int linebyte;
	BYTE* bmpBuffer = NULL;
	FILE *fp;
	if ((fp = fopen(charPath, "rb")) == NULL)  //以二进制的方式打开文件  
	{
		std::cout << "The file " << charPath << "was not opened" << std::endl;
		return FALSE;
	}
	if (fseek(fp, sizeof(BITMAPFILEHEADER), 0))  //跳过BITMAPFILEHEADE  
	{
		std::cout << "跳转失败" << std::endl;
		return FALSE;
	}
	BITMAPINFOHEADER infoHead;
	fread(&infoHead, sizeof(BITMAPINFOHEADER), 1, fp);   //从fp中读取BITMAPINFOHEADER信息到infoHead中
	texWidth = infoHead.biWidth;
	texHeight = infoHead.biHeight;
	linebyte = texWidth * 3; //24位颜色，一个颜色8位
	bmpBuffer = new BYTE[linebyte*texHeight];
	fread(bmpBuffer, sizeof(char), linebyte*texHeight, fp);
	Init();
	int index = 2;
	for (int i = 0; i < texHeight; i++)
	{
		for (int j = 0; j < texWidth; j++)
		{
			//b,g,r按这顺序存储的
			//textureData[j][i] = RGB(bmpBuffer[index - 0], bmpBuffer[index - 1], bmpBuffer[index - 2]);
			pixelColorData[j][i] = Color((float)bmpBuffer[index - 0] / 255.0f, (float)bmpBuffer[index - 1] / 255.0f, (float)bmpBuffer[index - 2] / 255.0f, 1.0f);
			index = index + 3;
		}
	}
	fclose(fp);   //关闭文件  
	//不确定是用delete还是用free，好像都可以
	delete [] bmpBuffer;
	//free(bmpBuffer);
	return TRUE;
}