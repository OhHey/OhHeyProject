#pragma once
//objects.h__________________________________

#include "objectmesh.h"

class GraphicsModule;
class objectmesh;

class gameobject {
public:
	objectmesh *mesh;
	GraphicsModule *pgraphics;

	ID3D11ShaderResourceView *texture;

	float x, y, w, h, vx, vy, aux1, aux2;

	float heading, turn;

	int type;

	gameobject(float x, float y, float w, float h, float vx, float vy, float xacc, float yacc, float pheading, float turn, int type, objectmesh *meshpara, GraphicsModule *pgraphicspara, ID3D11ShaderResourceView *texturepara);
	
};

