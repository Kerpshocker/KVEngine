#pragma once

//remove this header when we include our main dxgame file
#include <d3d11.h>

#include <DirectXMath.h>

using namespace DirectX; //needed for DXMath

struct Vertex
{
	XMFLOAT2 texture;
	XMFLOAT3 position;
	XMFLOAT4 color;
};

class Mesh
{
public:
	ID3D11Buffer *vBuffer;
	ID3D11Buffer *iBuffer;

	int indicesPerObject; //num to pass to draw

public:
	Mesh();
	~Mesh();

	void createMesh( ID3D11Device *device, Vertex vertices[], UINT indices[], int numIndices );
};

