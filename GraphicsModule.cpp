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

	InitLevel(1);
}

void GraphicsModule::Shutdown(void) {

}

// this is the function used to render
bool GraphicsModule::RenderFrame(float timeelapsed)
{
	HRESULT error;
	// clear the back buffer to a deep blue
	float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	thepipeline.devcon->ClearRenderTargetView(thepipeline.BackBuffer, clearColor);


	for(int i=0;i<gameobjects.size();i++){
		thepipeline.devcon->PSSetShaderResources(0,1,&gameobjects[i].texture);

		

		XMMATRIX trans( 
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		gameobjects[i].x,gameobjects[i].y,0,1);

		XMMATRIX scale( 
		gameobjects[i].w,0,0,0,
		0,gameobjects[i].h,0,0,
		0,0,50,0,
		0,0,0,1);

		XMMATRIX rot( 
		cos(gameobjects[i].heading * PI / 180),sin(gameobjects[i].heading * PI / 180),0,0,
		-sin(gameobjects[i].heading * PI / 180),cos(gameobjects[i].heading * PI / 180),0,0,
		0,0,1,0,
		0,0,0,1);

		XMMATRIX worldpaddle1 = rot*scale*trans;

		XMMATRIX WVP = worldpaddle1*view*proj;

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
		thepipeline.devcon->Draw(4, 0);
	}

	
	// switch the back buffer and the front buffer
	error = thepipeline.SwapChain->Present(0, 0);

	return true;//fix me
}

void GraphicsModule::InitLevel(int level){
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
	gameobject background(0,0,680,400,0,0,0,0,0,0,BACKDROP,&meshes[1],this,thepipeline.lvl1back1texture);
	gameobjects.push_back(background);
	gameobject ship(0,0,20,20,0,0,45,45,0,140,SHIP,&meshes[0],this,thepipeline.shiptexture);
	gameobjects.push_back(ship);

	
	gameobject asteroid(80,190,50,50,20,10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid2(-80,-190,50,50,30,10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid3(-80,190,50,50,-14,-10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid4(80,-190,50,50,20,-20,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobject asteroid5(230,90,50,50,-20,10,1,0,0,0,ASTEROID,&meshes[1],this,thepipeline.asttexture);
	gameobjects.push_back(asteroid);
	gameobjects.push_back(asteroid2);
	gameobjects.push_back(asteroid5);
	gameobjects.push_back(asteroid3);
	gameobjects.push_back(asteroid4);
}

void GraphicsModule::UpdateFrame(float timeelapsed, float totaltime){
	static float firedelay = cooldown;
	SetBackDrop(totaltime);

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
	if(GetAsyncKeyState(0x20)){ //Space bar
		firedelay = firedelay + timeelapsed;
		if(firedelay > cooldown){
			//gameobject projectile(gameobjects[0].x,gameobjects[0].y,5,5,(cos(gameobjects[0].heading*PI/180)*50)+gameobjects[0].vx,(sin(gameobjects[0].heading*PI/180)*50)+gameobjects[0].vy,gameobjects[0].x,gameobjects[0].y,gameobjects[0].heading,0,2,&meshes[1],this,thepipeline.projtexture);
			gameobject projectile(gameobjects[1].x,gameobjects[1].y,5,5,(cos(gameobjects[1].heading*PI/180)*50)+gameobjects[1].vx,(sin(gameobjects[1].heading*PI/180)*50)+gameobjects[1].vy,totaltime,0,0,0,PROJECTILE,&meshes[1],this,thepipeline.projtexture);
			gameobjects.push_back(projectile);
			firedelay = 0;
		}

	}

	for(int i = 0;i<gameobjects.size();i++){
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

	gameobject dummy(0,0,0,0,0,0,0,0,0,0,99,&meshes[1],this,thepipeline.projtexture);
	gameobjects.push_back(dummy);

	std::vector<int> nukelist;
	std::vector<gameobject> makelist;

	for(int i = 0;i<gameobjects.size();i++){
		if(gameobjects[i].type == 2){
			//if(sqrt(pow(abs(gameobjects[i].x-gameobjects[i].aux1),2)+pow(abs(gameobjects[i].y-gameobjects[i].aux2),2))>firingrange){
			if((totaltime - gameobjects[i].aux1)>firingrange){
				float teest = sqrt(pow(abs(gameobjects[i].x-gameobjects[i].aux1),2)+pow(abs(gameobjects[i].y-gameobjects[i].aux2),2));
				nukelist.push_back(i);
			}
		}
	}

	DeleteObjects(nukelist);
	nukelist.clear();
	gameobjects.push_back(dummy);

	for(int i = 0;i<gameobjects.size();i++){
		if(gameobjects[i].type == 2){
			for(int j = 0;j<gameobjects.size();j++){
				if(gameobjects[j].type == 3){
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
					}
				}
			}
		}
	}
	DeleteObjects(nukelist);
	nukelist.clear();

	for(int i = 0;i<makelist.size();i++){
		gameobjects.push_back(makelist[i]);
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

// this is the function that cleans up Direct3D and COM
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
