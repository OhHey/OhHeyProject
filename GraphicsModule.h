#pragma once
//GameModule.h__________________________________

#include "objects.h"
#include "pipeline.h"
#include "GameTimer.h"
#include <vector>

using namespace DirectX;


// function prototypes
void CleanD3D(void);         // closes Direct3D and releases memory

class GraphicsModule {
public:
	GraphicsModule(HWND window);

	pipeline thepipeline;

	std::vector<gameobject> gameobjects;
	
	std::vector<objectmesh> meshes;

	void InitLevel(int level);

	void UpdateFrame(float timeelapsed);
	bool RenderFrame(float timeelapsed);
	void Shutdown(void);

	XMVECTOR updir;
	XMVECTOR focus;
	XMVECTOR eyepos;
	XMMATRIX view;
	XMMATRIX proj;
};

