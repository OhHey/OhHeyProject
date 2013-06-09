#pragma once

#include <vector>
#include "structsandincludes.h"

class objectmesh {
public:
	ID3D11Buffer *VertexBuffer;
	D3D11_BUFFER_DESC BufferDesc;
	std::vector<VERTEXTEX> vertices;
	int numvertices;
	objectmesh(std::vector<VERTEXTEX>& paravertices);
};