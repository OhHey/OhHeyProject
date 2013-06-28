#pragma once
//GameModule.h__________________________________

#include "objects.h"
#include "pipeline.h"
#include "GameTimer.h"
#include "structsandincludes.h"
#include "physicslib.h"
#include <vector>

using namespace DirectX;

enum object_type { SHIP = 1, PROJECTILE = 2, ASTEROID = 3, DECORATION = 4, SHIELD = 5, UI = 95, TEXTOVER = 96, BACKDROP = 98, DUMMY = 99};
enum UI_type {ARMOURTEXT = 1, SHIELDTEXT = 2, ARMOURSTAT = 3, SHIELDSTAT = 4};
// function prototypes
void CleanD3D(void);         // closes Direct3D and releases memory

class GraphicsModule {
public:
	GraphicsModule(HWND window);

	pipeline thepipeline;

	std::vector<gameobject> gameobjects;
	std::vector<gameobject> UIobjects;
	std::vector<objectmesh> meshes;

	void InitLevel(int level);

	void InitMeshes();
	void InitLevel1();
	void InitUI();
	void UpdateFrame(float timeelapsed, float totaltime);
	void UpdateUI();
	void RenderUI();
	void RenderObjects();
	bool RenderFrame(float timeelapsed);
	void SetBackDrop(float totaltime);
	void GetInput(float timeelapsed, float totaltime);
	void DeleteObjects(std::vector<int> paranukelist);
	void Shutdown(void);

	float cooldown, firingrange, armour, shield;

	XMVECTOR updir;
	XMVECTOR focus;
	XMVECTOR eyepos;
	XMMATRIX view;
	XMMATRIX proj;
};

