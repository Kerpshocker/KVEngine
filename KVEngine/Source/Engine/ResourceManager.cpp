#include "Mesh.h"
#include "ResourceManager.h"
#include <d3dcompiler.h>

namespace KVE
{
	ResourceManager::ResourceManager()
	{
	}


	ResourceManager::~ResourceManager()
	{
	}

	UINT ResourceManager::createMesh( ID3D11Device* device, const MeshParams& meshParams )
	{
		Mesh* mesh = new Mesh( device, meshParams );
		meshArray[ 0 ] = mesh;
		return 0;
	}

	void ResourceManager::loadShadersAndInputLayout( ID3D11Device* device, ShaderInputParams& shaderParams, PixelShader& pixelShader )
	{
		// Set up the vertex layout description
		// This has to match the vertex input layout in the vertex shader
		// We can't set up the input layout yet since we need the actual vert shader
		D3D11_INPUT_ELEMENT_DESC vDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		shaderParams.vertexDesc = &vDesc[ 0 ];
		shaderParams.vertexDescSize = 2;

		// Load Vertex Shader --------------------------------------
		ID3DBlob* vsBlob;
		D3DReadFileToBlob( L"PC_VShader.cso", &vsBlob );

		// Create the shader on the device
		//HR( device->CreateVertexShader(
		//	vsBlob->GetBufferPointer(),
		//	vsBlob->GetBufferSize(),
		//	NULL,
		//	&shaderParams.vertexShader ) );

		//// Before cleaning up the data, create the input layout
		/*HR( device->CreateInputLayout(
		shaderParams.vertexDesc,
		shaderParams.vertexDescSize,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&shaderParams.inputLayout ) );*/

		//// Clean up
		//ReleaseMacro( vsBlob );

		// Compile pixel shader shader
		ID3DBlob *psBlob;
		D3DReadFileToBlob( L"PC_PShader.cso", &psBlob );

		//// Create the shader on the device
		//HR( device->CreatePixelShader(
		//	psBlob->GetBufferPointer(),
		//	psBlob->GetBufferSize(),
		//	NULL,
		//	&pixelShader.pixelShader ) );

		//// Clean up
		//ReleaseMacro( psBlob );

		//// Constant buffers ----------------------------------------
		//D3D11_BUFFER_DESC cBufferDesc;
		////cBufferDesc.ByteWidth = sizeof( VSDataToConstantBuffer );
		//cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		//cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		//cBufferDesc.CPUAccessFlags = 0;
		//cBufferDesc.MiscFlags = 0;
		//cBufferDesc.StructureByteStride = 0;
		//HR( device->CreateBuffer(
		//	&cBufferDesc,
		//	NULL,
		//	&shaderParams.vsConstantBuffer ) );
	}
}