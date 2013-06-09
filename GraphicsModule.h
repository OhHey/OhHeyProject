#pragma once
//GameModule.h__________________________________

#include "objects.h"
#include "pipeline.h"
#include "GameTimer.h"
#include "structsandincludes.h"
#include "physicslib.h"
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

	void DeleteObjects(std::vector<int> paranukelist);
	void UpdateFrame(float timeelapsed, float totaltime);
	bool RenderFrame(float timeelapsed);
	void Shutdown(void);

	float cooldown, firingrange;

	XMVECTOR updir;
	XMVECTOR focus;
	XMVECTOR eyepos;
	XMMATRIX view;
	XMMATRIX proj;
};

