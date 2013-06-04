#pragma once
//GameTimer.h__________________________________

#include <time.h>
#include <d3d11.h>

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class gametimer {
public:
	LARGE_INTEGER start, end, freq;
	gametimer::gametimer();
	void gametimer::starttimer();
	float gametimer::gettime();
};