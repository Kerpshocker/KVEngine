#include "GameManager.h"
#include "CameraManager.h"
#include "RenderManager.h"
#include "FrameManager.h"
#include "GameTimer.h"
#include <d3dcompiler.h>

using namespace DirectX;

void GameManager::initialize(const DXWindow* window, const GameTimer* timer)
{
	m_Timer = timer;

	KVE::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;
	cParams.initialPos = XMFLOAT3( 0.0f, 0.0f, -5.0f );
	CameraManager::Instance().createNewCamera( cParams, true );
	CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	CameraManager::Instance().getActiveCamera()->setViewMatrix();

	createShaders();
	createGeometry();

	Manager::initialize();
}

void GameManager::update( void )
{
	if ( !FrameManager::Instance().openFrame( &m_CurrentFrame ) ) return;

	m_CurrentFrame->DeltaTime = m_Timer->deltaTime();
	m_CurrentFrame->StartTime = m_Timer->totalTime();

	m_CurrentFrame->ViewMatrix = CameraManager::Instance().getActiveCamera()->getViewMatrix();
	m_CurrentFrame->ProjMatrix = CameraManager::Instance().getActiveCamera()->getProjMatrix();
	XMStoreFloat4x4( &m_CurrentFrame->WorldMatrix, XMMatrixIdentity() );

	m_LocalInstances[ 0 ].Position.x += 0.0001f;
	m_CurrentFrame->Instances = m_LocalInstances;
	m_CurrentFrame->InstanceStride = sizeof( MeshInstance );
	m_CurrentFrame->InstanceCount = m_LocalInstanceCount;

	m_CurrentFrame->EndTime = m_Timer->totalTime();

	FrameManager::Instance().closeFrame( &m_CurrentFrame );
}

void GameManager::createShaders( void )
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		// vertex buffer
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// instance buffer
		{ "POSITION", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	KVE::ShaderProgramDesc spDesc;
	spDesc.VShaderFile = L"PC_VShader.cso";
	spDesc.PShaderFile = L"PC_PShader.cso";
	spDesc.InputDesc = inputDesc;
	spDesc.NumVertexElements = ARRAYSIZE( inputDesc );

	RenderManager::Instance().createShaderLayout( spDesc );
}

void GameManager::createGeometry( void )
{
	//// Set up the vertices
	//KVE::Vertex vertices[] =
	//{
	//	{ XMFLOAT3( +0.0f, +1.0f, +0.0f ) },
	//	{ XMFLOAT3( -1.5f, -1.0f, +0.0f ) },
	//	{ XMFLOAT3( +1.5f, -1.0f, +0.0f ) },
	//};

	//// Set up the indices
	//UINT indices[] = { 0, 2, 1 };

	// Set up the instances
	XMFLOAT4 red = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 green = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 blue = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );
	m_LocalInstanceCount = 3;
	m_LocalInstances = new MeshInstance[ m_LocalInstanceCount ];
	m_LocalInstances[ 0 ] = { XMFLOAT3( -1.5f, -1.0f, 0.0f ), red };
	m_LocalInstances[ 1 ] = { XMFLOAT3( +1.5f, -1.0f, 0.0f ), green };
	m_LocalInstances[ 2 ] = { XMFLOAT3( 0.0f, 1.0f, 0.0f ), blue };

    KVE::ShaderBuffersDesc sbDesc;
   /* sbDesc.Vertices = vertices;
    sbDesc.VertexCount = ARRAYSIZE( vertices );
    sbDesc.VertexStride = sizeof( KVE::Vertex );
	sbDesc.VertexOffset = 0;
    sbDesc.VertexIndices = indices;
	sbDesc.VertexIndexCount = ARRAYSIZE( indices );*/
	KVE::createSBDescFromOBJFile( "crate_obj.obj", &sbDesc, sizeof( Vertex ) );
	sbDesc.InstanceCount = m_LocalInstanceCount;
	sbDesc.InstanceStride = sizeof( MeshInstance );
	sbDesc.InstanceOffset = 0;
    sbDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    RenderManager::Instance().createShaderBuffers( sbDesc, 0 );
}