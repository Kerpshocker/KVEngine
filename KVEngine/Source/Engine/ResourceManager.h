#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <DirectXMath.h>
#include "DataTypes.h"

class Mesh;

namespace KVE
{
	struct MeshParams
	{
		u32 vertexCount;
		u32* indices;
		u32 indexCount;

		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
	};

	struct MaterialParams
	{
		ID3D11ShaderResourceView *srv;
		ID3D11SamplerState *ss;
	};

	struct PixelShader
	{
		ID3D11PixelShader* pixelShader;
	};

	struct ShaderInputParams
	{
		ID3D11VertexShader* vertexShader;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* vsConstantBuffer;
		D3D11_INPUT_ELEMENT_DESC* vertexDesc;
		u32 vertexDescSize;
		void* dataToVSConstantBuffer;
	};

	class ResourceManager
	{
	public:
		Mesh* meshArray[ 1 ];
	public:
		ResourceManager();
		~ResourceManager();

		UINT createMesh( ID3D11Device* device, const MeshParams& meshParams );
		void loadShadersAndInputLayout( ID3D11Device* device, ShaderInputParams& shaderParams, PixelShader& pixelShader );
	};
}

#endif