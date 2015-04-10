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
	CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	CameraManager::Instance().getActiveCamera()->setViewMatrix();

	createShaders();
	createGeometry();

	Manager::initialize();
}

void GameManager::update( void )
{

}

void GameManager::createShaders( void )
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ShaderProgramDesc spDesc;
	spDesc.VShaderFile = L"PC_VShader.cso";
	spDesc.PShaderFile = L"PC_PShader.cso";
	spDesc.VertexDesc = vertexDesc;
	spDesc.NumVertexElements = 2;

	RenderManager::Instance().createShaderProgram( spDesc );
}

void GameManager::createGeometry( void )
{
	XMFLOAT4 red = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 green = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 blue = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );

	// Set up the vertices
	Vertex vertices[] =
	{
		{ XMFLOAT3( +0.0f, +1.0f, +0.0f ), red },
		{ XMFLOAT3( -1.5f, -1.0f, +0.0f ), green },
		{ XMFLOAT3( +1.5f, -1.0f, +0.0f ), blue },
	};

	// Set up the indices
	UINT indices[] = { 0, 2, 1 };

	VSDataToConstantBuffer* vsDataToConstantBuffer = new VSDataToConstantBuffer();
	XMStoreFloat4x4( &vsDataToConstantBuffer->World, DirectX::XMMatrixIdentity() );
	vsDataToConstantBuffer->Proj = CameraManager::Instance().getActiveCamera()->getProjMatrix();
	vsDataToConstantBuffer->View = CameraManager::Instance().getActiveCamera()->getViewMatrix();

	ShaderBuffersDesc sbDesc;
	sbDesc.Vertices = vertices;
	sbDesc.VertexCount = ARRAYSIZE( vertices );
	sbDesc.VertexStride = sizeof( Vertex );
	sbDesc.VertexOffset = 0;
	sbDesc.Indices = indices;
	sbDesc.IndexCount = ARRAYSIZE( indices );
	sbDesc.ConstBufferData = vsDataToConstantBuffer;
	sbDesc.ConstBufferByteSize = sizeof( VSDataToConstantBuffer );

	sbDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	RenderManager::Instance().createShaderBuffers( sbDesc );
}