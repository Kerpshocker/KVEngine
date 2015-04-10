#include "GameManager.h"
#include "CameraManager.h"
#include "RenderManager.h"
#include <d3dcompiler.h>

using namespace DirectX;

void GameManager::initialize( const DXWindow* window )
{
	KVE::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;
	CameraManager::Instance().createNewCamera( cParams, true );

	createShaders( window );
	createGeometry( window );

	Manager::initialize();
}

void GameManager::update( void )
{

}

void GameManager::createShaders( const DXWindow* window )
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob( L"PC_VShader.cso", &vsBlob );

	// Create the shader on the device
	HR( window->m_Device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_ShaderProgram.VertexShader ) );

	// Before cleaning up the data, create the input layout
	HR( window->m_Device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE( vertexDesc ),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_ShaderProgram.InputLayout ) );

	// Clean up
	ReleaseMacro( vsBlob );

	// Compile pixel shader shader
	ID3DBlob *psBlob;
	D3DReadFileToBlob( L"PC_PShader.cso", &psBlob );

	// Create the shader on the device
	HR( window->m_Device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&m_ShaderProgram.PixelShader ) );

	// Clean up
	ReleaseMacro( psBlob );

	m_ShaderProgram.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	RenderManager::Instance().setShaderProgram( &m_ShaderProgram );
}

void GameManager::createGeometry( const DXWindow* window )
{
	XMFLOAT4 red	= XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 green	= XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 blue	= XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );

	// Set up the vertices
	Vertex vertices[] =
	{
		{ XMFLOAT3( +0.0f, +1.0f, +0.0f ), red },
		{ XMFLOAT3( -1.5f, -1.0f, +0.0f ), green },
		{ XMFLOAT3( +1.5f, -1.0f, +0.0f ), blue },
	};
	m_ShaderBuffers.VertexStride = sizeof( Vertex );
	m_ShaderBuffers.VertexOffset = 0;

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof( Vertex ) * 3; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	HR( window->m_Device->CreateBuffer( &vbd, &initialVertexData, &m_ShaderBuffers.VertexBuffer ) );

	// Set up the indices
	UINT indices[] = { 0, 2, 1 };
	m_ShaderBuffers.IndexCount = 3;

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT ) * 3; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	HR( window->m_Device->CreateBuffer( &ibd, &initialIndexData, &m_ShaderBuffers.IndexBuffer ) );

	CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	CameraManager::Instance().getActiveCamera()->setViewMatrix();

	VSDataToConstantBuffer vsDataToConstantBuffer;
	XMStoreFloat4x4( &vsDataToConstantBuffer.World, DirectX::XMMatrixIdentity() );
	vsDataToConstantBuffer.Proj = CameraManager::Instance().getActiveCamera()->getProjMatrix();
	vsDataToConstantBuffer.View = CameraManager::Instance().getActiveCamera()->getViewMatrix();

	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof( VSDataToConstantBuffer );
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR( window->m_Device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&m_ShaderBuffers.ConstantBuffer ) );

	// Update the constant buffer itself
	window->m_DeviceContext->UpdateSubresource(
		m_ShaderBuffers.ConstantBuffer,
		0,
		NULL,
		&vsDataToConstantBuffer,
		0,
		0 );

	RenderManager::Instance().setShaderBuffers( &m_ShaderBuffers );
}