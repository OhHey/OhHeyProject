#pragma once
#define D3D_DEBUG_INFO

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <time.h>
#include <DirectXTex.h>
#include <WICTextureLoader.h>


#define PI 3.14159265

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

struct VERTEX
{
	FLOAT X, Y, Z;      // position
	float Color[4];    // color
};

struct VERTEXTEX
{
	float X, Y, Z;      // position
	float tx, ty;
};

struct PHYSVERT
{
	float x,y,z;
};

struct CONSTBUFFWVP
{
	XMMATRIX cWVP;
};

struct InputStruct{
	bool spacedown;
	bool wdown;
	bool adown;
	bool sdown;
	bool ddown;
	bool shiftdown;
};

