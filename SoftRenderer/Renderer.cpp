#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <assert.h>
#include <math.h>
#include <conio.h> 
#include <fstream>
#include <string>
#include <vector>
//自建
#include "Vector4.h"
#include "Vector2.h"
#include "Vertex.h"
#include "Matrix4X4.h"
#include "Math3D.h"
#include "Transform.h"
#include "Texture2D.h"
#include "DirectionalLight.h"
#include "Color.h"

using namespace std;

#define PI 3.1415926f

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGTH 480

HBRUSH BgBrush;

//MatrixLookAt(Matrix4X4* pOut, const Vector4* eye, const Vector4* at, const Vector4* up)
typedef struct
{
	Vector4 eyePos;
	Vector4 at;
	Vector4 up;
} LookAt;

//MatrixPerspectiveFov(&perspective, PI / 180.0f * 60.0f, (float)width / (float)height, 1.0f, 17.0f);
typedef struct
{
	float fov;
	float aspect;
	float zNear;
	float zFar;
} PerspectiveCamera;

typedef struct 
{
	int index[3];
}VerticeIndex;

typedef struct
{
	vector<Vertex> vertexVec;
	vector<VerticeIndex> verticeIndexVec;
}OBJModel;

//帧缓存,深度缓存
typedef struct
{	
	Color frameBuffer;
	float depthBuffer;
} Buffer; 

typedef struct
{
	float A;
	float B;
	float C;
	float D;
} Plane;

typedef struct
{
	float A;
	float B;
	float C;
} Line;


char key;
static HWND hWnd;
static HDC hdc, hMemDC;
static int width = DEFAULT_WIDTH;
static int height = DEFAULT_HEIGTH;
static RECT rect = {0, 0, width, height};
static Vector4 zeroPos = { 0.0f, 0.0f, 0.0f, 1.0f };
static Vector4 eyePos = { 0.0f, 1.0f, -10.0f, 1.0f };
static Vector4 at = { 0.0f, 0.0f, 0.0f, 1.0f };
static Vector4 up = { 0.0f, 1.0f, 0.0f, 1.0f };

int RenderState = 1;
//观察属性
LookAt lookAt = { eyePos, at, up };
//透视相机属性
PerspectiveCamera mainCamera = { PI / 180.0f * 60.0f, (float)width / (float)height, 1.0f, 50.0f };

Buffer buffer[DEFAULT_WIDTH][DEFAULT_HEIGTH];


BITMAPINFO bmpInfo;
HBITMAP hbmp;
UINT sreenTotalBytes = width * height;
BYTE* pSreenBuffer = new BYTE[sreenTotalBytes];

int index = 0;
//读取OBJ模型,贴图
string cubeTexturePath = "./cat.bmp";
Texture2D cubeTexture;
string cubeOBJPath = "./cube.obj";
OBJModel cubeOBJ;

string minerTexturePath = "./KuangGong.bmp";
Texture2D minerTexture;
string minerOBJPath = "./KuangGong.obj";
OBJModel minerOBJ;

//全局直射光照
DirectionalLight dirLight = DirectionalLight(Vector4(1.0, 1.0, 1.0, 1.0), Vector4(0.0,0.0,0.0,0.0), WHITE, 1.0f);
//环境光
Color ambLightColor = WHITE;
bool lightOn = true;
bool enableZBuffer = true;

//GameObject的Transform属性
Transform cubeTransform = Transform(Vector4(-1, 0, 0, 1), Vector4(-45, 0, 0, 1), Vector4(2, 2, 2, 1));
Transform magicianTransform = Transform(Vector4(1, 0, 0, 1), Vector4(-90, 0, 0, 1), Vector4(2, 2, 2, 1));

//帧率
string frameLabel = "";


static HWND InitWindow(int x, int y, int width, int height, LPCTSTR caption);
static LRESULT CALLBACK Default_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void UpdateWindow();
void UpdateFrame(HDC &hdc);
void DrawOneFrame(HDC &hdc);
void Clear();
bool BackFaceCullingByNormal(Vertex p1, Vertex p2, Vertex p3);
void DrawTriangle(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3, Matrix4X4 m, Matrix4X4 v, Matrix4X4 p);
void LocalToWorld(Vertex& vertex, Matrix4X4 m);
void WorldToViewport(Vertex& vertex, Matrix4X4 v);
void SetProjectionTransform(Vertex& vertex, Matrix4X4 p);
bool Clip(Vertex v);
void TransformToScreen(Vertex& v);
void ScanlineFill(Texture2D* texture, Vertex left, Vertex right, int yIndex);
void ScreenSpaceLerpVertex(Vertex& v, Vertex v1, Vertex v2, float t);
void DrawTriangleTop(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3);
void DrawTriangleBottom(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3);
void TriangleRasterization(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3);
void Render(OBJModel* model, Texture2D* texture, Transform* transform);
void LightOn(DirectionalLight* light, Vertex &v);

//观察矩阵
//zaxis = normal(At - Eye)
//xaxis = normal(cross(Up, zaxis))
//yaxis = cross(zaxis, xaxis)
//
//xaxis.x           yaxis.x           zaxis.x          0
//xaxis.y           yaxis.y           zaxis.y          0
//xaxis.z           yaxis.z           zaxis.z          0
//- dot(xaxis, eye) - dot(yaxis, eye) - dot(zaxis, eye)  1
void MatrixLookAt(Matrix4X4* pOut, const Vector4* eye, const Vector4* at, const Vector4* up)
{
	Vector4 xAxis, yAxis, zAxis;
	//求zAxis
	Vector4::Vec_Minus(&zAxis, at, eye);
	//Vector4::Vec_Normalize(&zAxis);
	zAxis.Normalize();
	//求xAxis
	Vector4::Vec_Cross(&xAxis, up, &zAxis);
	//Vector4::Vec_Normalize(&xAxis);
	xAxis.Normalize();
	//求yAxis
	Vector4::Vec_Cross(&yAxis, &zAxis, &xAxis);
	//Vector4::Vec_Normalize(&yAxis);
	yAxis.Normalize();

	pOut->Set_Identity();

	pOut->m[0][0] = xAxis.x;
	pOut->m[0][1] = yAxis.x;
	pOut->m[0][2] = zAxis.x;

	pOut->m[1][0] = xAxis.y;
	pOut->m[1][1] = yAxis.y;
	pOut->m[1][2] = zAxis.y;

	pOut->m[2][0] = xAxis.z;
	pOut->m[2][1] = yAxis.z;
	pOut->m[2][2] = zAxis.z;

	pOut->m[3][0] = -Vector4::Dot(&xAxis, eye);
	pOut->m[3][1] = -Vector4::Dot(&yAxis, eye);
	pOut->m[3][2] = -Vector4::Dot(&zAxis, eye);
}

void MatrixLookAt(Matrix4X4* pOut, const LookAt* lookat)
{
	Vector4 xAxis, yAxis, zAxis;
	//求zAxis
	Vector4::Vec_Minus(&zAxis, &lookat->at, &lookat->eyePos);
	//Vector4::Vec_Normalize(&zAxis);
	zAxis.Normalize();
	//求xAxis
	Vector4::Vec_Cross(&xAxis, &lookat->up, &zAxis);
	//Vector4::Vec_Normalize(&xAxis);
	xAxis.Normalize();
	//求yAxis
	Vector4::Vec_Cross(&yAxis, &zAxis, &xAxis);
	//Vector4::Vec_Normalize(&yAxis);
	yAxis.Normalize();

	pOut->Set_Identity();

	pOut->m[0][0] = xAxis.x;
	pOut->m[0][1] = yAxis.x;
	pOut->m[0][2] = zAxis.x;

	pOut->m[1][0] = xAxis.y;
	pOut->m[1][1] = yAxis.y;
	pOut->m[1][2] = zAxis.y;

	pOut->m[2][0] = xAxis.z;
	pOut->m[2][1] = yAxis.z;
	pOut->m[2][2] = zAxis.z;

	pOut->m[3][0] = -Vector4::Dot(&xAxis, &lookat->eyePos);
	pOut->m[3][1] = -Vector4::Dot(&yAxis, &lookat->eyePos);
	pOut->m[3][2] = -Vector4::Dot(&zAxis, &lookat->eyePos);
}

//投影矩阵（透视投影）
//xScale     0          0               0
//0        yScale       0               0
//0          0       zf / (zf - zn)     1
//0          0   -zn*zf / (zf - zn)		0
//
//yScale = cot(fovY / 2)
//xScale = yScale / aspect ratio
void MatrixPerspectiveFov(Matrix4X4* pOut, float fovY, float aspect, float zNear, float zFar)
{
	pOut->Set_Zero();
	float yScale = 1.0f / (tanf(fovY / 2.0f));
	float xScale = yScale / aspect;

	pOut->m[0][0] = xScale;
	pOut->m[1][1] = yScale;
	pOut->m[2][2] = zFar / (zFar - zNear);
	pOut->m[3][2] = -zNear * zFar / (zFar - zNear);
	pOut->m[2][3] = 1.0f;
}

void MatrixPerspectiveFov(Matrix4X4* pOut, const PerspectiveCamera* cam)
{
	pOut->Set_Zero();
	float yScale = 1.0f / (tanf(cam->fov / 2.0f));
	float xScale = yScale / cam->aspect;

	pOut->m[0][0] = xScale;
	pOut->m[1][1] = yScale;
	pOut->m[2][2] = cam->zFar / (cam->zFar - cam->zNear);
	pOut->m[3][2] = -cam->zNear * cam->zFar / (cam->zFar - cam->zNear);
	pOut->m[2][3] = 1.0f;
}


//能响应键盘和鼠标输入，wParam参数是具体消息
static LRESULT CALLBACK Default_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return (0);
	case WM_DESTROY:
		PostQuitMessage(0);
		return (0);
	case WM_KEYDOWN:
		//cout << (char)wParam << endl;
		switch ((char)wParam)
		{
		case 'w':case 'W':
			lookAt.eyePos.y += 0.05f;
			break;
		case 's':case 'S':
			lookAt.eyePos.y -= 0.05f;
			break;
		case 'a':case 'A':
			lookAt.eyePos.x -= 0.05f;
			break;
		case 'd':case 'D':
			lookAt.eyePos.x += 0.05f;
			break;	
		case 32://空格
			enableZBuffer = !enableZBuffer;
			break;
		case 38://上
			cubeTransform.Position.y += 0.05f;
			break;
		case 40://下
			cubeTransform.Position.y -= 0.05f;
			break;
		case 37://左
			cubeTransform.Position.x -= 0.05f;
			break;
		case 39://右
			cubeTransform.Position.x += 0.05f;
			break;
		case 'l':case 'L': //开灯关灯
			lightOn = !lightOn;
			break;
		default:
			break;
		}
		return (0);
		//鼠标左键
	case WM_LBUTTONDOWN:

		return (0);
		break;
		//鼠标右键
	case WM_RBUTTONDOWN:

		return (0);
		break;
		//鼠标中键
	case WM_MBUTTONDOWN:

		return (0);
		break;
		//鼠标滚轮
	case WM_MOUSEWHEEL:
		short zDelta = HIWORD(wParam);
		lookAt.eyePos.z += (zDelta * 0.002f);
		return (0);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

static HWND InitWindow(int x, int y, int width, int height, LPCTSTR caption)
{
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(0);

	const int captionMaxLen = 256;
	assert(strlen(caption) < captionMaxLen);
	static TCHAR szAppName[captionMaxLen];
	strcpy_s(szAppName, caption);

	HWND         hwnd;
	//MSG          msg;
	WNDCLASSEX   wndclassex = { 0 };
	wndclassex.cbSize = sizeof(WNDCLASSEX);
	wndclassex.style = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc = Default_WndProc;
	wndclassex.cbClsExtra = 0;
	wndclassex.cbWndExtra = 0;
	wndclassex.hInstance = hInstance;
	wndclassex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclassex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wndclassex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclassex.hbrBackground = BgBrush;
	wndclassex.lpszMenuName = NULL;
	wndclassex.lpszClassName = szAppName;
	wndclassex.hIconSm = wndclassex.hIcon;

	if (!RegisterClassEx(&wndclassex))
	{
		MessageBox(NULL, TEXT("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindowEx(0,
		szAppName,
		TEXT(szAppName),
		WS_OVERLAPPED | WS_BORDER | WS_CAPTION |
		WS_SYSMENU | WS_MINIMIZEBOX,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_NORMAL);
	UpdateWindow(hwnd);
	return hwnd;
}

void UpdateWindowMsg()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//已知直线上的两点P1(X1,Y1) P2(X2,Y2)， P1 P2两点不重合。则直线的一般式方程AX+BY+C=0中，A B C分别等于：
//A = Y2 - Y1
//B = X1 - X2
//C = X2*Y1 - X1*Y2
Line CalculateLinearEquations(float X1, float Y1, float X2, float Y2)
{
	Line result;
	float A = Y2 - Y1;
	float B = X1 - X2;
	float C = X2*Y1 - X1*Y2;
	result = { A, B, C };
	return result;
}
//根据直线方程求X
float CalculateX(float y, const Line* line)
{
	float result;
	result = -(line->B * y + line->C) / line->A;
	return result;
}


void DrawLine(Vector2 start, Vector2 end, COLORREF color)
{
	int dx = end.x - start.x;
	int dy = end.y - start.y;
	int steps, k;
	float xIncrement, yIncrement, x = start.x, y = start.y;

	if (abs(dx) > abs(dy))
	{
		steps = abs(dx);
	}
	else
	{
		steps = abs(dy);
	}
	xIncrement = float(dx) / float(steps);
	yIncrement = float(dy) / float(steps);
	//SetPixel(hMemDC, roundf(x), roundf(y), color);
	buffer[(int)roundf(x)][(int)roundf(y)].frameBuffer = Color::ToColor(color);

	for (k = 0; k < steps; k++)
	{
		x += xIncrement;
		y += yIncrement;
		//SetPixel(hMemDC, roundf(x), roundf(y), color);
		buffer[(int)roundf(x)][(int)roundf(y)].frameBuffer = Color::ToColor(color);
	}
}

void DrawLine(const Vector4* start, const Vector4* end, COLORREF color)
{
	Vector2 s = { start->x, start->y };
	Vector2 e = { end->x, end->y };
	DrawLine(s, e, color);
}

void DrawTriangleWire(const Vector4* v0, const Vector4* v1, const Vector4* v2)
{
	DrawLine(v0, v1, GREEN_COLORREF);
	DrawLine(v1, v2, GREEN_COLORREF);
	DrawLine(v2, v0, GREEN_COLORREF);
}

void LocalToWorld(Vertex& vertex, Matrix4X4 m)
{
	vertex.point = vertex.point * m;
	vertex.normal = vertex.normal * m;
}

void WorldToViewport(Vertex& vertex, Matrix4X4 v)
{
	vertex.point = vertex.point * v;
}

void SetProjectionTransform(Vertex& vertex, Matrix4X4 p)
{
	vertex.point = vertex.point * p;
	vertex.rhw = 1.0f / vertex.point.w;
	vertex.uv1.x *= vertex.rhw;
	vertex.uv1.y *= vertex.rhw;
}

bool Clip(Vertex v)
{
	//cvv为 x-1,1  y-1,1  z0,1
	if (v.point.x >= -v.point.w && v.point.x <= v.point.w &&
		v.point.y >= -v.point.w && v.point.y <= v.point.w &&
		v.point.z >= 0.0f && v.point.z <= v.point.w)
	{
		return true;
	}
	return false;
}

void TransformToScreen(Vertex& v)
{
	if (v.point.w != 0)
	{
		//先进行透视除法，转到cvv
		v.point.Homogenize();
		//cvv到屏幕坐标
		v.point.x = (v.point.x + 1.0f) * 0.5f * width;
		v.point.y = (1.0f - v.point.y) * 0.5f * height;
	}
}

void ScanlineFill(Texture2D* texture, Vertex left, Vertex right, int yIndex)
{
	float lineWidth = right.point.x - left.point.x;
	float step = 1.0f;
	if (lineWidth > 0)
	{
		step = 1.0f / lineWidth;
	}
	else
	{
		return;
	}
	for (float x = left.point.x; x <= right.point.x; x += 0.5f)
	{
		int xIndex = (int)(x + 0.5f);
		if (xIndex >= 0 && xIndex < width)
		{
			float lerpFactor = 0;
			if (lineWidth != 0)
			{
				lerpFactor = (x - left.point.x) / lineWidth;
			}
			//1/z’与x’和y'是线性关系的
			float rhwTemp = Math3D::Lerp(left.rhw, right.rhw, lerpFactor);
			//if (rhwTemp > buffer[xIndex, yIndex]->depthBuffer)//使用1/z进行深度测试
			if (rhwTemp > buffer[xIndex][yIndex].depthBuffer)//使用1/z进行深度测试
			{//通过测试	
				float w = 1.0f / rhwTemp;
				//开关Zbuffer，这部分抄的别人的代码，ZBuffer这一块效果不正确
				//4月16日解决，原因是把buffer[xIndex][yIndex]，写成了buffer[xIndex, yIndex],通过实验，发现这俩不等价，[,]具体含义正在查
				//这个问题困扰了好几周，调了很久很久才发现
				if (enableZBuffer)
				{
					//int index = (xIndex, yIndex);
					//buffer[xIndex, yIndex]->depthBuffer = rhwTemp;
					buffer[xIndex][yIndex].depthBuffer = rhwTemp;
				}
				//uv 插值，求纹理颜色
				float u = Math3D::Lerp(left.uv1.x, right.uv1.x, lerpFactor) * w * (texture->texWidth - 1);
				float v = Math3D::Lerp(left.uv1.y, right.uv1.y, lerpFactor) * w * (texture->texHeight - 1);
				//纹理采样
				int uIndex = (int)roundf(u);
				int vIndex = (int)roundf(v);
				Math3D::Clamp(uIndex, 0, texture->texWidth - 1);
				Math3D::Clamp(vIndex, 0, texture->texHeight - 1);

				Color vertexColor = WHITE;
				Color texColor = texture->pixelColorData[uIndex][vIndex];

				if (lightOn)
				{
					vertexColor = Math3D::Lerp(left.color, right.color, lerpFactor);
				}
				buffer[xIndex][yIndex].frameBuffer = vertexColor * texColor;
			 }
		}
	}
}

void ScreenSpaceLerpVertex(Vertex& v, Vertex v1, Vertex v2, float t)
{
	v.rhw = Math3D::Lerp(v1.rhw, v2.rhw, t);
	//
	v.uv1.x = Math3D::Lerp(v1.uv1.x, v2.uv1.x, t);
	v.uv1.y = Math3D::Lerp(v1.uv1.y, v2.uv1.y, t);

	v.color = Math3D::Lerp(v1.color, v2.color, t);
}

void DrawTriangleTop(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3)
{
	for (float y = p1.point.y; y <= p3.point.y; y += 0.5f)
	{
		int yIndex = (int)roundf(y);
		if (yIndex >= 0 && yIndex < height)
		{
			float xl = (y - p1.point.y) * (p3.point.x - p1.point.x) / (p3.point.y - p1.point.y) + p1.point.x;
			float xr = (y - p2.point.y) * (p3.point.x - p2.point.x) / (p3.point.y - p2.point.y) + p2.point.x;

			float dy = y - p1.point.y;
			float t = dy / (p3.point.y - p1.point.y);
			//插值生成左右顶点
			Vertex new1;
			new1.point.x = xl;
			new1.point.y = y;
			ScreenSpaceLerpVertex(new1, p1, p3, t);
			//
			Vertex new2;
			new2.point.x = xr;
			new2.point.y = y;
			ScreenSpaceLerpVertex(new2, p2, p3, t);
			//扫描线填充
			if (new1.point.x < new2.point.x)
			{
				ScanlineFill(texture, new1, new2, yIndex);
			}
			else
			{
				ScanlineFill(texture, new2, new1, yIndex);
			}
		}
	}
}

void DrawTriangleBottom(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3)
{
	for (float y = p1.point.y; y <= p2.point.y; y += 0.5f)
	{
		int yIndex = (int)roundf(y);
		if (yIndex >= 0 && yIndex < height)
		{
			float xl = (y - p1.point.y) * (p2.point.x - p1.point.x) / (p2.point.y - p1.point.y) + p1.point.x;
			float xr = (y - p1.point.y) * (p3.point.x - p1.point.x) / (p3.point.y - p1.point.y) + p1.point.x;

			float dy = y - p1.point.y;
			float t = dy / (p2.point.y - p1.point.y);
			//插值生成左右顶点
			Vertex new1;
			new1.point.x = xl;
			new1.point.y = y;
			ScreenSpaceLerpVertex(new1, p1, p2, t);
			//
			Vertex new2;
			new2.point.x = xr;
			new2.point.y = y;
			ScreenSpaceLerpVertex(new2, p1, p3, t);
			//扫描线填充
			if (new1.point.x < new2.point.x)
			{
				ScanlineFill(texture, new1, new2, yIndex);
			}
			else
			{
				ScanlineFill(texture, new2, new1, yIndex);
			}
		}
	}
}



void TriangleRasterization(Texture2D* texture, Vertex p1, Vertex p2, Vertex p3)
{
	if (p1.point.y == p2.point.y)
	{
		if (p1.point.y < p3.point.y)
		{//平顶
			DrawTriangleTop(texture, p1, p2, p3);
		}
		else
		{//平底
			DrawTriangleBottom(texture, p3, p1, p2);
		}
	}
	else if (p1.point.y == p3.point.y)
	{
		if (p1.point.y < p2.point.y)
		{//平顶
			DrawTriangleTop(texture, p1, p3, p2);
		}
		else
		{//平底
			DrawTriangleBottom(texture, p2, p1, p3);
		}
	}
	else if (p2.point.y == p3.point.y)
	{
		if (p2.point.y < p1.point.y)
		{//平顶
			DrawTriangleTop(texture, p2, p3, p1);
		}
		else
		{//平底
			DrawTriangleBottom(texture, p1, p2, p3);
		}
	}
	else
	{//分割三角形
		Vertex top;

		Vertex bottom;
		Vertex middle;
		if (p1.point.y > p2.point.y && p2.point.y > p3.point.y)
		{
			top = p3;
			middle = p2;
			bottom = p1;
		}
		else if (p3.point.y > p2.point.y && p2.point.y > p1.point.y)
		{
			top = p1;
			middle = p2;
			bottom = p3;
		}
		else if (p2.point.y > p1.point.y && p1.point.y > p3.point.y)
		{
			top = p3;
			middle = p1;
			bottom = p2;
		}
		else if (p3.point.y > p1.point.y && p1.point.y > p2.point.y)
		{
			top = p2;
			middle = p1;
			bottom = p3;
		}
		else if (p1.point.y > p3.point.y && p3.point.y > p2.point.y)
		{
			top = p2;
			middle = p3;
			bottom = p1;
		}
		else if (p2.point.y > p3.point.y && p3.point.y > p1.point.y)
		{
			top = p1;
			middle = p3;
			bottom = p2;
		}
		else
		{
			//三点共线
			return;
		}
		//插值求中间点x
		float middlex = (middle.point.y - top.point.y) * (bottom.point.x - top.point.x) / (bottom.point.y - top.point.y) + top.point.x;
		float dy = middle.point.y - top.point.y;
		float t = dy / (bottom.point.y - top.point.y);
		//插值生成左右顶点
		Vertex newMiddle;
		newMiddle.point.x = middlex;
		newMiddle.point.y = middle.point.y;
		ScreenSpaceLerpVertex(newMiddle, top, bottom, t);

		//平底
		DrawTriangleBottom(texture, top, newMiddle, middle);
		//平顶
		DrawTriangleTop(texture, newMiddle, middle, bottom);
	}
}

//背面消隐，根据面法线和视线消除背面
bool BackFaceCullingByNormal(Vertex p1, Vertex p2, Vertex p3)
{
	Vector4 v1 = p2.point - p1.point;
	Vector4 v2 = p3.point - p2.point;
	Vector4 normal = v1 * v2;
	//由于在视空间中，所以相机点就是（0,0,0）
	Vector4 viewDir = p1.point - Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	if (Vector4::Dot(&normal, &viewDir) > 0)
	{
		return true;
	}
	return false;
}

//光照，全局平行光，Lambert光照模型
//I = Ia * Ka + Ip * Kd * (N·L)
//Ia * Ka环境光
//如果不加入环境光，背光面就是纯黑
void LightOn(DirectionalLight* light, Vertex &v)
{
	float I, Ia, Ip, Kd, cosTheta;
	Ia = 0.2f; //环境光强度
	Ip = light->intensity; //直射光强度
	Kd = 1.0f; //直射光反射系数
	cosTheta = Vector4::Dot(&v.normal, &(light->DirectInWorld())); //入射角
	Math3D::Clamp(cosTheta, 0.0f, 1.0f); 
	//I = Ia * Ka + Ip * Kd * cosTheta;
	v.color = ambLightColor * (Ia * Kd) + light->color * (Ip * Kd * cosTheta);
}

void DrawTriangle(Texture2D* texture, Vertex v1, Vertex v2, Vertex v3, Matrix4X4 m, Matrix4X4 v, Matrix4X4 p)
{
	//本地坐标——>世界坐标
	LocalToWorld(v1, m);
	LocalToWorld(v2, m);
	LocalToWorld(v3, m);

	//根据 法线点乘视线，剔除背面，这里，DX9书上，背面消隐是在观察坐标系中做的，但是我试验的结果是效果不太好，
	//有的面没有及时渲染，不知道为啥，所以现在我把这一步提到世界坐标系中来做，效果好多了
	/*if (BackFaceCullingByNormal(p1) == false)
	{
		return;
	}*/
	
	if (lightOn)
	{
		LightOn(&dirLight, v1);
		LightOn(&dirLight, v2);
		LightOn(&dirLight, v3);
	}

	//世界坐标——>观察坐标
	WorldToViewport(v1, v);
	WorldToViewport(v2, v);
	WorldToViewport(v3, v);

	//这个背面消隐，确实应该在观察坐标系中做，现在查出来之前为什么“试验的结果是效果不太好，
	//有的面没有及时渲染”，原因是Vector4 Vector4::operator* (const Vector4& b)重载Vector4 *号的公式，算法写错了
	//rz = x*b.y - y*b.x; 写成了rz = y*b.x - b.y*x;  所以呈现出来的效果很奇葩，更正了就没问题了
	if (BackFaceCullingByNormal(v1, v2, v3) == false)
	{
		return;
	}

	//投影
	SetProjectionTransform(v1, p);
	SetProjectionTransform(v2, p);
	SetProjectionTransform(v3, p);

	//裁剪
	if (Clip(v1) == false || Clip(v2) == false || Clip(v3) == false)
	{
		return;
	}

	//视口坐标系
	TransformToScreen(v1);
	TransformToScreen(v2);
	TransformToScreen(v3);

	//三角形光栅化
	TriangleRasterization(texture, v1, v2, v3);
}

//根据模型的顶点索引，开始一个一个画三角形，这里传入的是未经过任何三维变换的顶点数据
void Draw(OBJModel* model, Texture2D* texture, Matrix4X4 m, Matrix4X4 v, Matrix4X4 p)
{
	for (int i = 0; i < model->verticeIndexVec.size(); i++)
	{
		int index0 = model->verticeIndexVec[i].index[0] - 1;
		int index1 = model->verticeIndexVec[i].index[1] - 1;
		int index2 = model->verticeIndexVec[i].index[2] - 1;
		DrawTriangle(texture, model->vertexVec[index0], model->vertexVec[index1], model->vertexVec[index2], m, v, p);
	}
}

//把渲染流程产出的结果buffer.frameBuffer，也就是每个像素的颜色，传入到屏幕缓存pSreenBuffer
void FillBuffer()
{
	index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//SetPixel效率巨低,弃用了
			//SetPixel(hMemDC, i, j, buffer[i][j].frameBuffer);
			COLORREF clref = buffer[j][i].frameBuffer.ToCOLORREF();
			//这里,之前我是按照r,g,b的顺序赋值的，后来发现颜色不对，倒过来之后就对了，原因如下：
			//#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
			//首先将r,g,b强制转换成BYTE型，之后g左移8位，b左移16位，并把结果分别强制转换成DWORD型，最后将r,左移8位后的g，
			//还有左移16位后的b三者做按位或，所得的结果强制转换成COLORREF类型。---------百度百科
			//也就是说，这里存储顺序是b,g,r，所以在给pBuffer赋值的时候需要按这个顺序，结果是这样的,b0,g0,r0,b1,g1,r1...
			pSreenBuffer[index] = GetBValue(clref);
			pSreenBuffer[index + 1] = GetGValue(clref);
			pSreenBuffer[index + 2] = GetRValue(clref);;
			index = index + 3;
		}
	}
}

//渲染部分，传入参数:模型，贴图，transform，在这里做各种三维变换、三角形光栅化，产出结果是整个屏幕所有像素点颜色的buffer
void Render(OBJModel* model, Texture2D* texture, Transform* transform)
{
	transform->Rotation.y += 1.0f;

	Matrix4X4 m, v, p;
	m.Set_Identity();
	v.Set_Identity();
	p.Set_Identity();
	m = Math3D::SetScale(transform->Scale.x, transform->Scale.y, transform->Scale.z)
		* Math3D::Translate(transform->Position.x, transform->Position.y, transform->Position.z)
		* Math3D::RotateX(transform->Rotation.x) * Math3D::RotateY(transform->Rotation.y) * Math3D::RotateZ(transform->Rotation.z);
	MatrixLookAt(&v, &lookAt);
	MatrixPerspectiveFov(&p, &mainCamera);

	Draw(model, texture, m, v, p);
}

//每帧update
void UpdateFrame(HDC &hdc)
{
	Clear();
	hMemDC = CreateCompatibleDC(hdc);
	//替代SetPixel的方案
	hbmp = CreateDIBSection(hMemDC, &bmpInfo, DIB_RGB_COLORS, (LPVOID *)&pSreenBuffer, NULL, 0); //创建DIB
	SelectObject(hMemDC, hbmp);
	FillRect(hMemDC, &rect, BgBrush);
	Render(&cubeOBJ, &cubeTexture, &cubeTransform);
	Render(&minerOBJ, &minerTexture, &magicianTransform);
	FillBuffer();
	//char* text = "Hello";
	//TextOut(hMemDC, 50, 50, (LPCTSTR)text, sizeof(text));
	//核心代码，将在后台绘制好的dc拷贝到前台显示dc
	BitBlt(hdc, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);

	//删除资源，防止泄漏
	DeleteDC(hMemDC);
	DeleteObject(hbmp);
}

//切分字符串
void StringSplit(string s, char splitchar, vector<string>& vec)
{
	if (vec.size()>0)//保证vec是空的  
		vec.clear();
	int length = s.length();
	int start = 0;
	for (int i = 0; i < length; i++)
	{
		if (s[i] == splitchar && i == 0)//第一个就遇到分割符  
		{
			start += 1;
		}
		else if (s[i] == splitchar)
		{
			vec.push_back(s.substr(start, i - start));
			start = i + 1;
		}
		else if (i == length - 1)//到达尾部  
		{
			vec.push_back(s.substr(start, i + 1 - start));
		}
	}
}

//读取OBJ模型文件，存入OBJModel
void ReadOBJFile(string path, OBJModel* obj)
{
	ifstream in(path);
	int vertexIndex = 0;
	int vertexCount = 0;
	string txt = "";

	if (in) // 有该文件  
	{
		while (getline(in, txt)) // line中不包括每行的换行符  
		{	
			//顶点坐标
			if (txt[0] == 'v' && txt[1] == ' ')
			{
				vector<string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				Vertex newVertex;
				newVertex.point = { (float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()), 1.0 };
				obj->vertexVec.push_back(newVertex);
				vertexCount++;
			}
			//顶点法线
			else if (txt[0] == 'v' && txt[1] == 'n')
			{
				vector<string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				obj->vertexVec[vertexIndex].normal = Vector4((float)atof(num[0].c_str()), (float)atof(num[1].c_str()), (float)atof(num[2].c_str()), 0.0);
				vertexIndex++;
				if (vertexIndex == vertexCount)
				{
					vertexIndex = 0;
				}
			}
			//uv
			else if (txt[0] == 'v' && txt[1] == 't')
			{
				vector<string> num;
				txt.erase(0, 3);
				StringSplit(txt, ' ', num);
				obj->vertexVec[vertexIndex].uv1 = Vector2((float)atof(num[0].c_str()), (float)atof(num[1].c_str()));
				obj->vertexVec[vertexIndex].rhw = 1.0f;
				vertexIndex++;
				if (vertexIndex == vertexCount)
				{
					vertexIndex = 0;
				}
				//obj.stVec.push_back({ (float)atof(num[0].c_str()), (float)atof(num[1].c_str())});
			}
			//索引编号
			//　"f 7/13/21"这时在面的数据中多了贴图坐标uv点和法线的索引号，索引号分别用左斜线(/)隔开。 
			else if (txt[0] == 'f' && txt[1] == ' ')
			{
				vector<string> num;
				txt.erase(0, 2);
				StringSplit(txt, ' ', num);
				vector<string> index;
				for (int i = 0; i < num.size(); i ++)
				{
					vector<string> threeIndex;
					StringSplit(num[i], '/', threeIndex);
					index.push_back(threeIndex[i]);
				}
				VerticeIndex indexes = { atoi(index[0].c_str()), atoi(index[1].c_str()), atoi(index[2].c_str()) };
				obj->verticeIndexVec.push_back(indexes);
			}
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
}

//刷新缓存区所有像素和深度值
void Clear()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			buffer[j][i].frameBuffer = BackgroundColor;
			buffer[j][i].depthBuffer = 0.0f;
		}
	}
}

int main(int argc, char* argv[])
{
	BgBrush = CreateSolidBrush(BackgroundColorREF);
	hWnd = InitWindow(500, 500, width, height, argv[0]);
	hdc = GetWindowDC(hWnd);

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;//宽度
	bmpInfo.bmiHeader.biHeight = -height;//高度
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = height * width * 3;
	
	cubeTexture.LoadBmp(cubeTexturePath);
	ReadOBJFile(cubeOBJPath, &cubeOBJ);
	minerTexture.LoadBmp(minerTexturePath);
	ReadOBJFile(minerOBJPath, &minerOBJ);

	while (1)
	{
		UpdateWindowMsg();
		UpdateFrame(hdc);
	}
	return 0;
}