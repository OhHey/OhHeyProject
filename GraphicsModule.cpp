//GameModule.cpp__________________________________
#include "GraphicsModule.h"

GraphicsModule:: GraphicsModule(HWND window):thepipeline(window)
{
	XMVECTOR updirt= {0,1,0};
	XMVECTOR focust= {0,0,0};
	XMVECTOR eyepost= {0,0,-450};

	updir = updirt;
	focus = focust;
	eyepos = eyepost;

	XMMATRIX viewt = XMMatrixLookAtLH(eyepos,focus,updir);
	XMMATRIX projt = XMMatrixPerspectiveFovLH((3.14*0.25),(800.0f/600.0f),1,1000);

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

		gameobjects[i].y = gameobjects[i].y + (gameobjects[i].vy * timeelapsed);

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
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1);

		XMMATRIX worldpaddle1 = rot*scale*trans;

		XMMATRIX WVP = worldpaddle1*view*proj;

		// select which vertex buffer to display
		UINT stride = sizeof(VERTEXTEX);
		UINT offset = 0;	
	
		thepipeline.devcon->IASetVertexBuffers(0, 1, &gameobjects[i].mesh->VertexBuffer, &stride, &offset);
		// select which primtive type we are using
		thepipeline.devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


		thepipeline.constbuffshader.cWVP = WVP;

		error = thepipeline.devcon->Map(thepipeline.cb, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &(thepipeline.constbuffmvp));
		if (FAILED(error)) { MessageBox(NULL,L"OH NOES CONSTBUFFMAP!",L"QUIT",NULL); }

		memcpy(thepipeline.constbuffmvp.pData, &(thepipeline.constbuffshader), sizeof(thepipeline.constbuffshader));                // copy the data

		thepipeline.devcon->Unmap(thepipeline.cb, NULL);													// unmap the buffer

		// draw the vertex buffer to the back buffer
		thepipeline.devcon->Draw(4, 0);
	}

	
	// switch the back buffer and the front buffer
	error = thepipeline.SwapChain->Present(0, 0);

	return true;//fix me
}

void GraphicsModule::InitLevel(int level){

	objectmesh squaremesh;
	meshes.push_back(squaremesh);
	thepipeline.CreateVB(meshes[0]);
	gameobject temp(0,0,20,20,0,0,&meshes[0],this,thepipeline.pShaderRVBricks);
	gameobjects.push_back(temp);
	
}

void GraphicsModule::UpdateFrame(float timeelapsed){
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
