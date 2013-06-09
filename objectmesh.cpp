#include "objectmesh.h"

objectmesh::objectmesh(std::vector<VERTEXTEX>& paravertices){
	HRESULT error;

	vertices = paravertices;

	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	// Square Vertex Buffer Textured
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	BufferDesc.ByteWidth = sizeof(VERTEXTEX) * vertices.size();			// size is the VERTEX struct * 4
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer*/

	// create a square with texture coords using the VERTEXTEX struct
}