#include "objectmesh.h"

objectmesh::objectmesh(){
	HRESULT error;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	// Square Vertex Buffer Textured
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	BufferDesc.ByteWidth = sizeof(VERTEXTEX) * 4;			// size is the VERTEX struct * 4
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer*/

	// create a square with texture coords using the VERTEXTEX struct
	VERTEXTEX setup2[4] = 
	{
		{0.0f, -1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
		{1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 0.0f, 1.0f, 0.0f}
	};	

	vertices[0] = setup2[0];
	vertices[1] = setup2[1];
	vertices[2] = setup2[2];
	vertices[3] = setup2[3];	
}