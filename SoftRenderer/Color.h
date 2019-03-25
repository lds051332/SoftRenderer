#ifndef  COLOR_H
#define  COLOR_H
#include <windows.h>

#define WHITE_COLORREF  RGB(255, 255, 255)
#define RED_COLORREF  RGB(255, 0, 0)
#define GREEN_COLORREF  RGB(0, 255, 0)
#define BLUE_COLORREF  RGB(0, 0, 255)
#define YELLOW_COLORREF  RGB(255, 255, 0)
#define BLACK_COLORREF  RGB(0, 0, 0)
#define CYAN_COLORREF  RGB(0, 255, 255)
#define MAGENTA_COLORREF  RGB(255, 0, 255) 
#define BackgroundColorREF  RGB(49, 77, 121)

#define WHITE Color(1.0, 1.0, 1.0, 1.0)
#define RED Color(1.0, 0.0, 0.0, 1.0)
#define GREEN Color(0.0, 1.0, 0.0, 1.0)
#define BLUE Color(0.0, 0.0, 1.0, 1.0)
#define YELLOW Color(1.0, 1.0, 0.0, 1.0)
#define BLACK Color(0.0, 0.0, 0.0, 1.0)
#define CYAN Color(0.0, 1.0, 1.0, 1.0)
#define MAGENTA Color(1.0, 0.0, 1.0, 1.0)
#define BackgroundColor Color(0.19, 0.3, 0.47, 1.0)

class Color
{
public:
	Color();
	Color(float r, float g, float b, float a);
	~Color();
	float r;
	float g;
	float b;
	float a;

	//和COLORREF互相转换
	static Color ToColor(const COLORREF& clRef);
	COLORREF ToCOLORREF();

	//重载运算符
	void operator= (const Color& rValue);
	Color operator* (const Color& rValue);
	Color operator+ (const Color& rValue);
	Color operator- (const Color& rValue);
	Color operator* (const float& rValue);

private:
	void ClampColor();
	Color ClampColor(Color& cl);
};
#endif

