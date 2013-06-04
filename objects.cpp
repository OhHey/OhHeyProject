//objects.cpp__________________________________
#include "objects.h"



gameobject::gameobject(float px, float py, float pw, float ph, float pvxb, float pvyb, objectmesh *meshpara, GraphicsModule *pgraphicspara, ID3D11ShaderResourceView *texturepara){
	x = px;
	y = py;
	w = pw;
	h = ph;
	vxb = pvxb;
	vyb = pvyb;
	vx = 7;
	vy = 0;
	mesh = meshpara;
	pgraphics = pgraphicspara;
	texture = texturepara;
}





/*void gameobject::update(float timeelapsed){
	
	static float pospaddlex1 = -70;
	static float pospaddlex2 = 50;

	static float pospaddley1 = 0;
	static float pospaddley2 = 0;

	static int score1v = 0;
	static int score2v = 0;

	float normalx, normaly, remainingtime;
	int checksum = 0;

	//Ball movement and collision detection

	//First check is ball is scored

	if(ball.x < -210){
		ball.x = 0;
		ball.vxb = -1*(rand() % 70 + 40);
		score2v++;
	}
	if(ball.x > 240){
		ball.x = 0;
		ball.vxb = -1*(rand() % 60 + 30);
		score1v++;
	}

	ball.vx = elapsedTime*ball.vxb;
	ball.vy = elapsedTime*ball.vyb;

	float collisiontime[4];
	collisiontime[0] = SweptAABB(ball, walltop, normalx, normaly);
	collisiontime[1] = SweptAABB(ball, wallbot, normalx, normaly);
	collisiontime[2] = SweptAABB(ball, paddle1, normalx, normaly);
	collisiontime[3] = SweptAABB(ball, paddle2, normalx, normaly);

	for(int i=0;i<4;i++){
		if(collisiontime[i] != 1){
			ball.x += ball.vx * (collisiontime[i]);
			ball.y += ball.vy * (collisiontime[i]);
			remainingtime = 1.0f - collisiontime[i];
			if(normalx != 0){
				ball.vx = -ball.vx;
				ball.vxb = -ball.vxb;
				if(i == 2)
					ball.vyb += paddle1.vy/5;
				if(i == 3)
					ball.vyb += paddle2.vy/5;
				ball.x += ball.vx * (remainingtime);
				ball.y += ball.vy * (remainingtime);
			}
			if(normaly != 0){
				ball.vy = -ball.vy;
				ball.vyb = -ball.vyb;
				ball.x += ball.vx * (remainingtime);
				ball.y += ball.vy * (remainingtime);
			}
		}
		else
			checksum++;
	}

	if(checksum == 4){
		ball.x += ball.vx;
		ball.y += ball.vy;
	}

	//Paddle2 AI

	float ballmid = ball.y - (ball.h/2);
	float paddle2mid = paddle2.y - (paddle2.h/2);

	if((ball.vx > 0) && (ball.x > 15)){
		if(ballmid > paddle2mid)
			paddle2.vy = 50;
		if(ballmid < paddle2mid)
			paddle2.vy = -50;
	}
	else{
		if(paddle2mid < -50)
			paddle2.vy = 50;
		if(paddle2mid > -50)
			paddle2.vy = -50;
	}
}*/