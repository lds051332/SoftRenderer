#include "Color.h"
#include <math.h>
#include "Math3D.h"

//限制颜色值分量在0-1之间

void Color::ClampColor()
{
	Math3D::Clamp(r, 0.0f, 1.0f);
	Math3D::Clamp(g, 0.0f, 1.0f);
	Math3D::Clamp(b, 0.0f, 1.0f);
	Math3D::Clamp(a, 0.0f, 1.0f);
}

Color Color::ClampColor(Color& cl)
{
	Math3D::Clamp(cl.r, 0.0f, 1.0f);
	Math3D::Clamp(cl.g, 0.0f, 1.0f);
	Math3D::Clamp(cl.b, 0.0f, 1.0f);
	Math3D::Clamp(cl.a, 0.0f, 1.0f);
	return cl;
}


Color::Color()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 0.0f;
}

Color::Color(float rV, float gV, float bV, float aV)
{
	r = rV;
	g = gV;
	b = bV;
	a = aV;
	ClampColor();
}

Color::~Color()
{
}

void Color::operator= (const Color& rValue)
{
	r = rValue.r;
	g = rValue.g;
	b = rValue.b;
	a = rValue.a;
	ClampColor();
}

Color Color::operator* (const Color& rValue)
{
	float rx, ry, rz, rw;
	rx = r*rValue.r;
	ry = g*rValue.g;
	rz = b*rValue.b;
	rw = a*rValue.a;
	return ClampColor(Color(rx, ry, rz, rw));
}

Color Color::operator+ (const Color& rValue)
{
	float rx, ry, rz, rw;
	rx = r + rValue.r;
	ry = g + rValue.g;
	rz = b + rValue.b;
	rw = a + rValue.a;
	return ClampColor(Color(rx, ry, rz, rw));
}

Color Color::operator- (const Color& rValue)
{
	float rx, ry, rz, rw;
	rx = r - rValue.r;
	ry = g - rValue.g;
	rz = b - rValue.b;
	rw = a - rValue.a;
	return ClampColor(Color(rx, ry, rz, rw));
}

Color Color::operator* (const float& rValue)
{
	float rx, ry, rz, rw;
	rx = r * rValue;
	ry = g * rValue;
	rz = b * rValue;
	rw = a * rValue;
	return ClampColor(Color(rx, ry, rz, rw));
}

COLORREF Color::ToCOLORREF()
{
	ClampColor();
	COLORREF re;
	re = RGB(r * 255, g * 255, b * 255);
	return re;
}

Color Color::ToColor(const COLORREF& clRef)
{
	Color re;
	re.r = (float)GetRValue(clRef) / 255.0f;
	re.g = (float)GetGValue(clRef) / 255.0f;
	re.b = (float)GetBValue(clRef) / 255.0f;
	re.a = 1.0f;
	return re;
}