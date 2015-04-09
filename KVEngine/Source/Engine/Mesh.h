#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "DataTypes.h"
#include "ResourceManager.h"

using namespace DirectX;

class Mesh
{
public:
	ID3D11Buffer *vBuffer;
	ID3D11Buffer *iBuffer;

	u32 indicesPerObject;
	u32 verticesPerObject;

	Mesh();
	Mesh( ID3D11Device* device, const KVE::MeshParams& meshParams );
	~Mesh();
};

