//objects.cpp__________________________________
#include "objects.h"



gameobject::gameobject(float px, float py, float pw, float ph, float pvx, float pvy, float pxacc, float pyacc, float pheading, float pturn, int ptype, objectmesh *meshpara, GraphicsModule *pgraphicspara, ID3D11ShaderResourceView *texturepara){
	x = px;
	y = py;
	w = pw;
	h = ph;
	aux1 = pxacc;
	aux2 = pyacc;
	vx = pvx;
	vy = pvy;
	heading = pheading;
	turn = pturn;
	mesh = meshpara;
	type = ptype;
	pgraphics = pgraphicspara;
	texture = texturepara;
}