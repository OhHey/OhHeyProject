#pragma once

#include "structsandincludes.h"

class objectmesh {
public:
	ID3D11Buffer *VertexBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	VERTEXTEX vertices[4];

	objectmesh();
};