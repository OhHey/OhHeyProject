#include "physicslib.h"

float SweptAABB(gameobject b1, gameobject b2, float& normalx, float& normaly){

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

bool circlecoll(gameobject c1, gameobject c2){
	if(pow((c1.w*0.5)+(c2.w*0.5),2)>(pow(c1.x-c2.x,2)+pow(c1.y-c2.y,2))) //if combined radius is less than distance between centers
		return true;
	else
		return false;
}

//float trianglecirclecoll(gameobject tri, gameobject circ){
bool sepaxistest(gameobject obj1, gameobject obj2)
{
	std::vector<PHYSVERT> vertices1;
	std::vector<PHYSVERT> vertices2;
	std::vector<PHYSVERT> normals;
	std::vector<int> indices1;
	std::vector<int> indices2;
	std::vector<float> minmax1;
	std::vector<float> minmax2;
	PHYSVERT temp;
	int i,j;

	XMMATRIX trans1( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		obj1.x,obj1.y,0,1);

	XMMATRIX scale1( 
		obj1.w,0,0,0,
		0,obj1.h,0,0,
		0,0,0,0,
		0,0,0,1);

	XMMATRIX rot1( 
		cos(obj1.heading * PI / 180),sin(obj1.heading * PI / 180),0,0,
		-sin(obj1.heading * PI / 180),cos(obj1.heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

	XMMATRIX world1 = rot1*scale1*trans1;

	XMMATRIX trans2( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		obj2.x,obj2.y,0,1);

	XMMATRIX scale2( 
		obj2.w,0,0,0,
		0,obj2.h,0,0,
		0,0,0,0,
		0,0,0,1);

	XMMATRIX rot2( 
		cos(obj2.heading * PI / 180),sin(obj2.heading * PI / 180),0,0,
		-sin(obj2.heading * PI / 180),cos(obj2.heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

	XMMATRIX world2 = rot2*scale2*trans2;

	getindices(obj1, indices1);
	getindices(obj2, indices2);


	for(i = 0;i < indices1.size();i++){//get vertices in real space
		XMVECTOR coords = {obj1.mesh->vertices[indices1[i]].X,obj1.mesh->vertices[indices1[i]].Y,obj1.mesh->vertices[indices1[i]].Z,1};
		coords = XMVector4Transform(coords,world1);
		temp.x = coords.m128_f32[0];
		temp.y = coords.m128_f32[1];
		vertices1.push_back(temp);
	}

	for(i = 0;i < indices2.size();i++){//get vertices in real space
		XMVECTOR coords = {obj2.mesh->vertices[indices2[i]].X,obj2.mesh->vertices[indices2[i]].Y,obj2.mesh->vertices[indices2[i]].Z,1};
		coords = XMVector4Transform(coords,world2);
		temp.x = coords.m128_f32[0];
		temp.y = coords.m128_f32[1];
		vertices2.push_back(temp);
	}

	for(i = 0;i < vertices1.size()-1;i++){
		temp.y = vertices1[i+1].x-vertices1[i].x;
		temp.x = -(vertices1[indices1[i+1]].y-vertices1[i].y);
		normals.push_back(temp);
	}

	for(i = 0;i < vertices2.size()-1;i++){
		temp.y = vertices2[i+1].x-vertices2[i].x;
		temp.x = -(vertices2[i+1].y-vertices2[i].y);
		normals.push_back(temp);
	}

	for(i = 0; i < normals.size();i++){//get projection intersects
		for(j = 0; j < indices1.size();j++){
			minmax1.push_back((vertices1[indices1[j]].x * normals[i].x)+(vertices1[indices1[j]].y*normals[i].y));
		}
		for(j = 0; j < indices2.size();j++){
			minmax2.push_back((vertices2[indices2[j]].x * normals[i].x)+(vertices2[indices2[j]].y*normals[i].y));
		}
		std::sort(minmax1.begin(),minmax1.begin()+minmax1.size());//sort intersects highest to lowest
		std::sort(minmax2.begin(),minmax2.begin()+minmax2.size());
		if((minmax2.front() > minmax1.back())&&(minmax2.back() > minmax1.back()))//
			return false;
		if((minmax2.front() < minmax1.front())&&(minmax2.back() < minmax1.front()))
			return false;
		minmax1.clear();
		minmax2.clear();
	}
	return true;
}

bool sepaxistestcircle(gameobject obj1, gameobject circle){
	std::vector<PHYSVERT> vertices1;
	PHYSVERT vertices2;
	PHYSVERT normal;
	std::vector<PHYSVERT> normals;
	std::vector<int> indices1;
	std::vector<float> minmax1, minmax2;
	
	PHYSVERT temp, closest;
	int i,j;

	XMMATRIX trans1( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		obj1.x,obj1.y,0,1);

	XMMATRIX scale1( 
		obj1.w,0,0,0,
		0,obj1.h,0,0,
		0,0,0,0,
		0,0,0,1);

	XMMATRIX rot1( 
		cos(obj1.heading * PI / 180),sin(obj1.heading * PI / 180),0,0,
		-sin(obj1.heading * PI / 180),cos(obj1.heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

	XMMATRIX world1 = rot1*scale1*trans1;

	getindices(obj1, indices1);

	for(i = 0;i < indices1.size();i++){//get vertices in real space
		XMVECTOR coords = {obj1.mesh->vertices[indices1[i]].X,obj1.mesh->vertices[indices1[i]].Y,obj1.mesh->vertices[indices1[i]].Z,1};
		coords = XMVector4Transform(coords,world1);
		temp.x = coords.m128_f32[0];
		temp.y = coords.m128_f32[1];
		vertices1.push_back(temp);
	}

	//GET CIRCLE AXIS AND CHECK
	float distance = sqrt(pow(vertices1[0].x-circle.x,2)+pow(vertices1[0].y-circle.y,2));
	closest = vertices1[0];
	for(i = 1;i < vertices1.size();i++){
		float newdistance = sqrt(pow(vertices1[i].x-circle.x,2)+pow(vertices1[i].y-circle.y,2));
		if(distance > newdistance){
			distance = newdistance;
			closest = vertices1[i];
		}
	}

	normal.x = closest.x-circle.x;
	normal.y = (closest.y-circle.y);

	float mag = sqrt(pow(normal.x,2)+pow(normal.y,2));
	normal.x = normal.x/mag;
	normal.y = normal.y/mag;

	for(j = 0; j < vertices1.size();j++){
		minmax1.push_back((vertices1[j].x * normal.x)+(vertices1[j].y*normal.y));
	}
	
	float centerproj = (circle.x * normal.x)+(circle.y*normal.y);
	minmax2.push_back(centerproj+circle.w/2);
	minmax2.push_back(centerproj-circle.w/2);
	
	std::sort(minmax1.begin(),minmax1.begin()+minmax1.size());//sort intersects highest to lowest
	std::sort(minmax2.begin(),minmax2.begin()+minmax2.size());

	if((minmax2.front() > minmax1.back())&&(minmax2.back() > minmax1.back()))//
		return false;
	if((minmax2.front() < minmax1.front())&&(minmax2.back() < minmax1.front()))
		return false;
	minmax1.clear();
	minmax2.clear();

	//CHECK OTHER AXIS
	for(i = 0;i < vertices1.size()-1;i++){
		temp.y = vertices1[i+1].x-vertices1[i].x;
		temp.x = -(vertices1[i+1].y-vertices1[i].y);
		mag = sqrt(pow(temp.x,2)+pow(temp.y,2));
		temp.x = temp.x/mag;
		temp.y = temp.y/mag;
		normals.push_back(temp);
	}

	for(i = 0; i < normals.size();i++){//get projection intersects
		for(j = 0; j < vertices1.size();j++){
			minmax1.push_back((vertices1[j].x * normals[i].x)+(vertices1[j].y*normals[i].y));
		}
		centerproj = (circle.x * normals[i].x)+(circle.y*normals[i].y);
		minmax2.push_back(centerproj+circle.w/2);
		minmax2.push_back(centerproj-circle.w/2);

		std::sort(minmax1.begin(),minmax1.begin()+minmax1.size());//sort intersects highest to lowest
		std::sort(minmax2.begin(),minmax2.begin()+minmax2.size());
		if((minmax2.front() > minmax1.back())&&(minmax2.back() > minmax1.back()))
			return false;
		if((minmax2.front() < minmax1.front())&&(minmax2.back() < minmax1.front()))
			return false;
		minmax1.clear();
		minmax2.clear();
	}

	return true;
}

void getindices(gameobject obj, std::vector<int>& indices){

	if(obj.mesh->vertices.size() == 3){
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
	}

	if(obj.mesh->vertices.size() == 4){
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(0);
	}

	if(obj.mesh->vertices.size() == 5){
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(6);
		indices.push_back(7);
	}
		
}