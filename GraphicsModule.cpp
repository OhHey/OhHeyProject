//GameModule.cpp__________________________________
#include "GraphicsModule.h"

GraphicsModule:: GraphicsModule(HWND window):thepipeline(window)
{
	XMVECTOR updirt= {0,1,0};
	XMVECTOR focust= {0,0,0};
	XMVECTOR eyepost= {0,0,450};

	updir = updirt;
	focus = focust;
	eyepos = eyepost;

	XMMATRIX viewt = XMMatrixLookAtRH(eyepos,focus,updir);
	XMMATRIX projt = XMMatrixPerspectiveFovRH((PI*0.25),(1280.0f/720.0f),1,1000);

	cooldown = 0.3;
	firingrange = 4;

	view = viewt;
	proj = projt;

	InitMeshes();
	InitUI();
	InitLevel(1);
}

void GraphicsModule::Shutdown(void) {

}

bool GraphicsModule::RenderFrame(float timeelapsed)
{
	HRESULT error;
	// clear the back buffer to a deep blue
	float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	thepipeline.devcon->ClearRenderTargetView(thepipeline.BackBuffer, clearColor);

	RenderObjects();
	RenderUI();

	error = thepipeline.SwapChain->Present(0, 0);

	return true;//fix me
}

void GraphicsModule::InitLevel(int level){

	for(int i = 0;i < UIobjects.size();i++){
		if(UIobjects[i].type == TEXTOVER)
			UIobjects.erase(UIobjects.begin() + i);
	}

	if(level == 1)
		InitLevel1();

}

void GraphicsModule::LevelOver(){
	gameobjects.clear();
	gameobject levelcomplete(0,0,150,50,0,0,0,0,0,0,TEXTOVER,&meshes[1],this,thepipeline.levelcompletetexture);
	UIobjects.push_back(levelcomplete);
	while(!GetAsyncKeyState(0x0D)){
		RenderUI();
		thepipeline.SwapChain->Present(0, 0);
	}
	InitLevel(1);
}

void GraphicsModule::UpdateFrame(float timeelapsed, float totaltime){

	numast = 0;
	
	SetBackDrop(totaltime);//Set stars on background

	UpdateUI();
	
	if(armour > 0)
		GetInput(timeelapsed, totaltime);//Deal with keystrokes
	

	for(int i = 0;i<gameobjects.size();i++){//Loop objects around screen
		if(gameobjects[i].x > 340)
			gameobjects[i].x = -340;
		if(gameobjects[i].x < -340)
			gameobjects[i].x = 340;
		if(gameobjects[i].y > 200)
			gameobjects[i].y = -200;
		if(gameobjects[i].y < -200)
			gameobjects[i].y = 200;
		gameobjects[i].x += timeelapsed * gameobjects[i].vx;
		gameobjects[i].y += timeelapsed * gameobjects[i].vy;
	}

	gameobject dummy(0,0,0,0,0,0,0,0,0,0,99,&meshes[1],this,thepipeline.projtexture);//add dummy object to stack
	gameobjects.push_back(dummy);

	std::vector<int> nukelist;
	std::vector<gameobject> makelist;

	//Check for projectiles reaching ranges
	for(int i = 0;i<gameobjects.size();i++){
		if(gameobjects[i].type == 2){
			//if(sqrt(pow(abs(gameobjects[i].x-gameobjects[i].aux1),2)+pow(abs(gameobjects[i].y-gameobjects[i].aux2),2))>firingrange){
			if((totaltime - gameobjects[i].aux1)>firingrange){
				float teest = sqrt(pow(abs(gameobjects[i].x-gameobjects[i].aux1),2)+pow(abs(gameobjects[i].y-gameobjects[i].aux2),2));
				nukelist.push_back(i);
			}
		}
		/*if(gameobjects[i].type == 97){
			if(sepaxistest(gameobjects[1],gameobjects[i]))
				gameobjects[i].texture = thepipeline.projtexture;
			else
				gameobjects[i].texture = thepipeline.shiptexture;
		}*/
	}

	DeleteObjects(nukelist);
	nukelist.clear();
	gameobjects.push_back(dummy);

	//Check for collisions
	for(int i = 0;i<gameobjects.size();i++){
		if(gameobjects[i].type == SHIELD){
			if(shield > 0){
				gameobjects[i].aux2 = 0;//Render the shield
				gameobjects[i].x = gameobjects[1].x;
				gameobjects[i].y = gameobjects[1].y;
				for(int j = 0;j<gameobjects.size();j++){
					if(gameobjects[j].type == ASTEROID){ //Test if shield hits asteroids
						if(circlecoll(gameobjects[i],gameobjects[j])){
							nukelist.push_back(j);
							if(gameobjects[j].aux1 == 1){
								gameobject asteroid2(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx+30,gameobjects[i].vy+10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid3(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx-14,gameobjects[i].vy-10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid4(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx+20,gameobjects[i].vy-20,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid5(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx-20,gameobjects[i].vy+10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								makelist.push_back(asteroid2);
								makelist.push_back(asteroid3);
								makelist.push_back(asteroid4);
								makelist.push_back(asteroid5);
							}
							shield--;
							gameobjects[1].vx = gameobjects[1].vx/2;
							gameobjects[1].vy = gameobjects[1].vy/2;
							DeleteObjects(nukelist);
							nukelist.clear();
							gameobjects.push_back(dummy);
						}
					}
				}
			}
			else{
				gameobjects[i].aux2 = -1;//Dont render the shield
			}
		}

		if(gameobjects[i].type == SHIP){ //Find ship type
			if(shield == 0){
				for(int j = 0;j<gameobjects.size();j++){
					if(gameobjects[j].type == ASTEROID){ //Test if ship hits asteroids
						if(sepaxistestcircle(gameobjects[i],gameobjects[j])){
							nukelist.push_back(j);
							if(gameobjects[j].aux1 == 1){
								gameobject asteroid2(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx+30,gameobjects[i].vy+10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid3(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx-14,gameobjects[i].vy-10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid4(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx+20,gameobjects[i].vy-20,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								gameobject asteroid5(gameobjects[j].x,gameobjects[j].y,30,30,gameobjects[i].vx-20,gameobjects[i].vy+10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
								makelist.push_back(asteroid2);
								makelist.push_back(asteroid3);
								makelist.push_back(asteroid4);
								makelist.push_back(asteroid5);
							}
							armour--;
							gameobjects[i].vx = gameobjects[i].vx/2;
							gameobjects[i].vy = gameobjects[i].vy/2;
							DeleteObjects(nukelist);
							nukelist.clear();
							gameobjects.push_back(dummy);
						}
					}
				}
			}
		}

		if(gameobjects[i].type == PROJECTILE){
			for(int j = 0;j<gameobjects.size();j++){
				if(gameobjects[j].type == ASTEROID){
					if(circlecoll(gameobjects[i],gameobjects[j])){
						if(gameobjects[j].aux1 == 1){
							gameobject asteroid2(gameobjects[j].x,gameobjects[j].y,30,30,30,10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
							gameobject asteroid3(gameobjects[j].x,gameobjects[j].y,30,30,-14,-10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
							gameobject asteroid4(gameobjects[j].x,gameobjects[j].y,30,30,20,-20,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
							gameobject asteroid5(gameobjects[j].x,gameobjects[j].y,30,30,-20,10,2,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
							makelist.push_back(asteroid2);
							makelist.push_back(asteroid3);
							makelist.push_back(asteroid4);
							makelist.push_back(asteroid5);
						}
						nukelist.push_back(i);
						nukelist.push_back(j);
						DeleteObjects(nukelist);
						nukelist.clear();
						gameobjects.push_back(dummy);
					}
				}
			}
		}
		
		if(gameobjects[i].type == ASTEROID){
			numast++;
		}
	}	

	for(int i = 0;i<makelist.size();i++){
		gameobjects.push_back(makelist[i]);
	}

	if(numast == 0){
		LevelOver();
	}
}

void GraphicsModule::DeleteObjects(std::vector<int> paranukelist){

	for(int i = 0;i<paranukelist.size();i++){
		std::iter_swap(gameobjects.begin()+paranukelist[i],gameobjects.end()-1);
		gameobjects.erase(gameobjects.end()-1);
	}

	for(int i = 0;i<gameobjects.size();i++){
		if(gameobjects[i].type == 99){
			gameobjects.erase(gameobjects.begin()+i);
			break;
		}
	}
}

void GraphicsModule::SetBackDrop(float totaltime){

	float backgroundtimer = (int)totaltime % 4;

	if(backgroundtimer<1){
		gameobjects[0].texture = thepipeline.lvl1back1texture;
	}
	else if(backgroundtimer<2){
		gameobjects[0].texture = thepipeline.lvl1back2texture;
	}
	else if(backgroundtimer<3){
		gameobjects[0].texture = thepipeline.lvl1back3texture;
	}
	else{
		gameobjects[0].texture = thepipeline.lvl1back4texture;
	}
}

/*
void CleanD3D()
{
	// close and release all existing COM objects
	pLayouttex->Release();
	cb->Release();
	pVBuffertri->Release();
	pVS->Release();
	pPS->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}
*/

void GraphicsModule::GetInput(float timeelapsed, float totaltime){
	static float firedelay = cooldown;

	if(GetAsyncKeyState(0x20)){ //Space bar
		firedelay = firedelay + timeelapsed;
		if(firedelay > cooldown){
			//gameobject projectile(gameobjects[0].x,gameobjects[0].y,5,5,(cos(gameobjects[0].heading*PI/180)*50)+gameobjects[0].vx,(sin(gameobjects[0].heading*PI/180)*50)+gameobjects[0].vy,gameobjects[0].x,gameobjects[0].y,gameobjects[0].heading,0,2,&meshes[1],this,thepipeline.projtexture);
			gameobject projectile(gameobjects[1].x,gameobjects[1].y,5,5,(cos(gameobjects[1].heading*PI/180)*50)+gameobjects[1].vx,(sin(gameobjects[1].heading*PI/180)*50)+gameobjects[1].vy,totaltime,0,0,0,PROJECTILE,&meshes[1],this,thepipeline.projtexture);
			gameobjects.push_back(projectile);
			firedelay = 0;
		}

	}
	if(GetAsyncKeyState(0x26)){ //Up arrow
		gameobjects[1].vx += timeelapsed * cos(gameobjects[1].heading * PI / 180)*gameobjects[1].aux1;
		//gameobjects[0].vx = 40;
		gameobjects[1].vy += timeelapsed * sin(gameobjects[1].heading * PI / 180)*gameobjects[1].aux2;
	}
	if(GetAsyncKeyState(0x25)) //Left arrow
		gameobjects[1].heading += timeelapsed * gameobjects[1].turn;

	if(GetAsyncKeyState(0x27)) //Right arrow
		gameobjects[1].heading -= timeelapsed * gameobjects[1].turn;

	if(GetAsyncKeyState(0x28)){ //Down arrow
		//gameobjects[0].vy = 40;
		gameobjects[1].vx -= timeelapsed * cos(gameobjects[1].heading * PI / 180)*gameobjects[1].aux1;
		gameobjects[1].vy -= timeelapsed * sin(gameobjects[1].heading * PI / 180)*gameobjects[1].aux2;
	}
}

void GraphicsModule::UpdateUI(){
	if(armour == 0){//Check if dead
		gameobject gameover(0,0,150,50,0,0,0,0,0,0,TEXTOVER,&meshes[1],this,thepipeline.gameovertexture);
		UIobjects.push_back(gameover);
	}
	for(int i = 0;i < UIobjects.size();i++){
		if(UIobjects[i].type == ARMOURSTAT){
			if(armour == 0)
				UIobjects[i].texture = thepipeline.text0texture;
			if(armour == 1)
				UIobjects[i].texture = thepipeline.text1texture;
			if(armour == 2)
				UIobjects[i].texture = thepipeline.text2texture;
			if(armour == 3)
				UIobjects[i].texture = thepipeline.text3texture;
		}
		if(UIobjects[i].type == SHIELDSTAT){
			if(shield == 0)
				UIobjects[i].texture = thepipeline.text0texture;
			if(shield == 1)
				UIobjects[i].texture = thepipeline.text1texture;
			if(shield == 2)
				UIobjects[i].texture = thepipeline.text2texture;
			if(shield == 3)
				UIobjects[i].texture = thepipeline.text3texture;
		}		
	}
}

void GraphicsModule::RenderUI(){
	HRESULT error;
	for(int i=0;i<UIobjects.size();i++){
		thepipeline.devcon->PSSetShaderResources(0,1,&UIobjects[i].texture);

		XMMATRIX trans( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		UIobjects[i].x,UIobjects[i].y,0,1);

		XMMATRIX scale( 
		UIobjects[i].w,0,0,0,
		0,UIobjects[i].h,0,0,
		0,0,0,0,
		0,0,0,1);

		XMMATRIX rot( 
		cos(UIobjects[i].heading * PI / 180),sin(UIobjects[i].heading * PI / 180),0,0,
		-sin(UIobjects[i].heading * PI / 180),cos(UIobjects[i].heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

		XMMATRIX world = rot*scale*trans;

		XMMATRIX WVP = world*view*proj;

		// select which vertex buffer to display
		UINT stride = sizeof(VERTEXTEX);
		UINT offset = 0;	
	
		thepipeline.devcon->IASetVertexBuffers(0, 1, &UIobjects[i].mesh->VertexBuffer, &stride, &offset);
		thepipeline.devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		thepipeline.constbuffshader.cWVP = WVP;

		error = thepipeline.devcon->Map(thepipeline.cb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &(thepipeline.constbuffmvp));
		if (FAILED(error)) { MessageBox(NULL,L"OH NOES CONSTBUFFMAP!",L"QUIT",NULL); }
		memcpy(thepipeline.constbuffmvp.pData, &(thepipeline.constbuffshader), sizeof(thepipeline.constbuffshader));               
		thepipeline.devcon->Unmap(thepipeline.cb, NULL);													

		// draw the vertex buffer to the back buffer
		thepipeline.devcon->Draw(UIobjects[i].mesh->vertices.size(), 0);
	}
}

void GraphicsModule::RenderObjects(){
	HRESULT error;

	for(int i=0;i<gameobjects.size();i++){
		if(gameobjects[i].aux2 == -1){;}
		else{
		thepipeline.devcon->PSSetShaderResources(0,1,&gameobjects[i].texture);

		XMMATRIX trans( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		gameobjects[i].x,gameobjects[i].y,0,1);

		XMMATRIX scale( 
		gameobjects[i].w,0,0,0,
		0,gameobjects[i].h,0,0,
		0,0,0,0,
		0,0,0,1);

		XMMATRIX rot( 
		cos(gameobjects[i].heading * PI / 180),sin(gameobjects[i].heading * PI / 180),0,0,
		-sin(gameobjects[i].heading * PI / 180),cos(gameobjects[i].heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

		XMMATRIX world = rot*scale*trans;

		XMMATRIX WVP = world*view*proj;

		// select which vertex buffer to display
		UINT stride = sizeof(VERTEXTEX);
		UINT offset = 0;	
	
		thepipeline.devcon->IASetVertexBuffers(0, 1, &gameobjects[i].mesh->VertexBuffer, &stride, &offset);
		thepipeline.devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		thepipeline.constbuffshader.cWVP = WVP;

		error = thepipeline.devcon->Map(thepipeline.cb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &(thepipeline.constbuffmvp));
		if (FAILED(error)) { MessageBox(NULL,L"OH NOES CONSTBUFFMAP!",L"QUIT",NULL); }
		memcpy(thepipeline.constbuffmvp.pData, &(thepipeline.constbuffshader), sizeof(thepipeline.constbuffshader));               
		thepipeline.devcon->Unmap(thepipeline.cb, NULL);													

		// draw the vertex buffer to the back buffer
		thepipeline.devcon->Draw(gameobjects[i].mesh->vertices.size(), 0);
	}
	}
}

void GraphicsModule::InitMeshes(){
	float blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	float red[4] = {1.0f, 0.0f, .0f, 1.0f};
	float green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	
	
	/*VERTEX square[4] =
	{
		{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, .0f, 1.0f},
		{0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
		{0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f}
	};*/

	VERTEXTEX square[4] =
	{
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f, 0.0f, 0.0f},
		{0.5f, -0.5f, 0.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.0f, 1.0f, 0.0f}
	};
	
	VERTEXTEX triangle[3] = 
	{
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f, 0.0f, 0.0f},
		{0.5f, 0.0f, 0.0f, 1.0f, 0.5f}		
	};

	std::vector<VERTEXTEX> temptriangle(triangle,triangle+sizeof(triangle)/sizeof(VERTEXTEX));
	std::vector<VERTEXTEX> tempsquare(square,square+sizeof(square)/sizeof(VERTEXTEX));
	//std::vector<VERTEX> temp(setup,setup+sizeof(setup)/sizeof(VERTEX));
	objectmesh trianglemesh(temptriangle);
	objectmesh squaremesh(tempsquare);
	meshes.push_back(temptriangle);
	meshes.push_back(squaremesh);
	thepipeline.CreateVB(meshes[0]);
	thepipeline.CreateVB(meshes[1]);
}

void GraphicsModule::InitLevel1(){

	gameobject background(0,0,680,400,0,0,0,0,0,0,BACKDROP,&meshes[1],this,thepipeline.lvl1back1texture);
	gameobjects.push_back(background);

	gameobject ship(0,0,20,20,0,0,45,45,0,140,SHIP,&meshes[0],this,thepipeline.shiptexture);
	gameobject shieldobj(0,0,30,30,0,0,0,0,0,0,SHIELD,&meshes[1],this,thepipeline.shieldtexture);
	gameobjects.push_back(ship);
	gameobjects.push_back(shieldobj);

	//gameobject colltest(80,90,50,50,0,0,1,0,0,0,97,&meshes[1],this,thepipeline.shiptexture);
	//gameobjects.push_back(colltest);
	
	gameobject asteroid(20,90,50,50,5,40,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid2(-80,-190,50,50,30,10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid3(-80,190,50,50,-14,-10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid4(80,-190,50,50,20,-20,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid5(230,90,50,50,-20,10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobjects.push_back(asteroid);
	gameobjects.push_back(asteroid2);
	gameobjects.push_back(asteroid5);
	gameobjects.push_back(asteroid3);
	gameobjects.push_back(asteroid4);

	shield = 3;
	armour = 3;
}

void GraphicsModule::InitUI(){
	gameobject shieldtext(-250,-150,100,33,0,0,0,0,0,0,SHIELDTEXT,&meshes[1],this,thepipeline.shieldtexttexture);
	gameobject armourtext(-250,-170,100,33,0,0,0,0,0,0,ARMOURTEXT,&meshes[1],this,thepipeline.armourtexttexture);
	gameobject shieldstat(-180,-150,100,33,0,0,0,0,0,0,SHIELDSTAT,&meshes[1],this,thepipeline.text0texture);
	gameobject armourstat(-180,-170,100,33,0,0,0,0,0,0,ARMOURSTAT,&meshes[1],this,thepipeline.text3texture);
	UIobjects.push_back(armourstat);
	UIobjects.push_back(shieldstat);
	UIobjects.push_back(shieldtext);
	UIobjects.push_back(armourtext);
}