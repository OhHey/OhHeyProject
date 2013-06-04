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

	float x, y, w, h, vx, vy, vxb, vyb;

	int type;

	gameobject(float x, float y, float w, float h, float vxb, float vyb, objectmesh *meshpara, GraphicsModule *pgraphicspara, ID3D11ShaderResourceView *texturepara);
	
};

float inline SweptAABB(gameobject b1, gameobject b2, float& normalx, float& normaly){

	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	// find the distance between the objects on the near and far sides for both x and y
	if (b1.vx > 0.0f)
	{
		xInvEntry = b2.x - (b1.x + b1.w);
		xInvExit = (b2.x + b2.w) - b1.x;
	}
	else 
	{
		xInvEntry = (b2.x + b2.w) - b1.x;
		xInvExit = b2.x - (b1.x + b1.w);
	}

	if (b1.vy > 0.0f)
	{
		yInvEntry = (b2.y - b2.h) - b1.y;
		yInvExit = b2.y - (b1.y - b1.h);
	}
	else
	{
		yInvEntry = b2.y - (b1.y - b1.h);
		yInvExit = (b2.y - b2.h) - b1.y;
	}
	if(yInvEntry < 1 && yInvEntry > 0)
		int ran = yInvEntry;

	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
	float xEntry, yEntry;
	float xExit, yExit;

	if (b1.vx == 0.0f)
	{
		//xEntry = -std::numeric_limits<float>::infinity();
		xEntry = 1.1;
		xExit = 1.1;
		//xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / b1.vx;
		xExit = xInvExit / b1.vx;
	}

	if (b1.vy == 0.0f)
	{
		//yEntry = -std::numeric_limits<float>::infinity();
		//yExit = std::numeric_limits<float>::infinity();
		yEntry = 1.1;
		yExit = 1.1;
	}
	else
	{
		yEntry = yInvEntry / b1.vy;
		yExit = yInvExit / b1.vy;
	}

	// find the earliest/latest times of collision
	float exitTime, entryTime;
	if(xEntry > yEntry)
		entryTime = xEntry;
	else
		entryTime = yEntry;

	if(xExit > yExit)
		exitTime = yExit;
	else
		exitTime = xExit;

	// if there was no collision
	if ((entryTime > exitTime) || (xEntry < 0.0f && yEntry < 0.0f) || xEntry > 1.0f || yEntry > 1.0f)
	{
		//normalx = 0.0f;
		//normaly = 0.0f;
		return 1.0f;
	}

	else // if there was a collision
	{        		
		// calculate normal of collided surface
		if (xEntry > yEntry)
		{
			if (xInvEntry < 0.0f)
			{
				normalx = 1.0f;
				normaly = 0.0f;
			}
			else
			{
				normalx = -1.0f;
				normaly = 0.0f;
			}
		}
		else
		{
			if (yInvEntry < 0.0f)
			{
				normalx = 0.0f;
				normaly = 1.0f;
			}
			else
			{
				normalx = 0.0f;
				normaly = -1.0f;
			}
		}

		// return the time of collision
		return entryTime;
	}
}