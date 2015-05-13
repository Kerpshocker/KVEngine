#include "GameManager.h"
#include "CameraManager.h"
#include "RenderManager.h"
#include "FrameManager.h"
#include "InputManager.h"
#include "GameTimer.h"
#include "OBB.h"
#include <d3dcompiler.h>

using namespace DirectX;

void GameManager::initialize( const KVE::Graphics::DXWindow* window, const KVE::System::GameTimer* timer )
{
	m_Timer = timer;

	KVE::Graphics::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;
	cParams.initialPos = XMFLOAT3( 0.0f, 0.0f, -5.0f );
	KVE::Graphics::CameraManager::Instance().createNewCamera( cParams, true );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setViewMatrix();

	createShaders();
	createGeometry();

	Manager::initialize();
}

void GameManager::update( void )
{
	KVE::Graphics::FrameManager::Instance().openFrame( &m_CurrentFrame );

	m_CurrentFrame->StartTime = m_Timer->totalTime();
	m_CurrentFrame->DeltaTime = m_CurrentFrame->StartTime - m_LastFrameEndTime;

	m_Player.update( (float)m_CurrentFrame->DeltaTime );

	m_CurrentFrame->ViewMatrix = KVE::Graphics::CameraManager::Instance().getActiveCamera()->getViewMatrix();
	m_CurrentFrame->ProjMatrix = KVE::Graphics::CameraManager::Instance().getActiveCamera()->getProjMatrix();
	XMStoreFloat4x4( &m_CurrentFrame->WorldMatrix, XMMatrixIdentity() );

	m_LocalInstances[ 0 ].Position.x += 0.0001f;
	m_CurrentFrame->Instances = m_LocalInstances;
	m_CurrentFrame->InstanceStride = sizeof( MeshInstance );
	m_CurrentFrame->InstanceCount = m_LocalInstanceCount;

	m_LastFrameEndTime = m_CurrentFrame->EndTime = m_Timer->totalTime();

	KVE::Graphics::FrameManager::Instance().closeFrame( &m_CurrentFrame );
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

	KVE::Graphics::ShaderProgramDesc spDesc;
	spDesc.VShaderFile = L"PC_VShader.cso";
	spDesc.PShaderFile = L"PC_PShader.cso";
	spDesc.InputDesc = inputDesc;
	spDesc.NumVertexElements = ARRAYSIZE( inputDesc );

	KVE::Graphics::RenderManager::Instance().createShaderLayout( spDesc, 1 );
}

void GameManager::createGeometry( void )
{
	// Set up the instances
	XMFLOAT4 red = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 green = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 blue = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );
	m_LocalInstanceCount = 3;
	m_LocalInstances = new MeshInstance[ m_LocalInstanceCount ];
	m_LocalInstanceCount = 3;
	m_LocalInstances = new MeshInstance[ m_LocalInstanceCount ];
	m_LocalInstances[ 0 ] = { XMFLOAT3( -1.5f, -1.0f, 0.0f ), red };
	m_LocalInstances[ 1 ] = { XMFLOAT3( +1.5f, -1.0f, 0.0f ), green };
	m_LocalInstances[ 2 ] = { XMFLOAT3( 0.0f, 1.0f, 0.0f ), blue };

	KVE::Graphics::ShaderBuffersDesc meshSBDesc;
	KVE::Graphics::createSBDescFromOBJFile( "crate_obj.obj", &meshSBDesc, sizeof( Vertex ) );
	meshSBDesc.InstanceCount = m_LocalInstanceCount;
	meshSBDesc.InstanceStride = sizeof( MeshInstance );
	meshSBDesc.InstanceOffset = 0;
	meshSBDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	KVE::Graphics::RenderManager::Instance().createShaderBuffers( meshSBDesc, 0 );

	m_LocalOABBInstanceCount = 1;
	m_LocalOABBInstances = new OABBInstance[ m_LocalOABBInstanceCount ];
	m_LocalOABBInstances[ 0 ].Position = m_LocalInstances[ 0 ].Position;
	m_LocalOABBInstances[ 0 ].Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );

	XMVECTOR frontTopRight = XMVectorSet( 0.5f, 0.5f, -0.5f, 0.0f );
	XMVECTOR backBottomLeft = XMVectorSet( -0.5f, -0.5f, 0.5f, 0.0f );

	XMVECTOR* oabbPosition = new XMVECTOR();
	*oabbPosition = XMLoadFloat3( &m_LocalInstances[ 0 ].Position );

	KVE::Collisions::OBB obb = KVE::Collisions::OBB( oabbPosition, frontTopRight, backBottomLeft );

	KVE::Graphics::ShaderBuffersDesc oabbSBDesc;
	oabbSBDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	oabbSBDesc.Vertices = obb.getCollisionCorners();
	oabbSBDesc.VertexCount = 8;
	oabbSBDesc.VertexOffset = 0;
	oabbSBDesc.VertexStride = sizeof( Vertex );
	oabbSBDesc.VertexIndexCount = 8;
	oabbSBDesc.VertexIndices = obb.getIndices();
	oabbSBDesc.InstanceCount = m_LocalOABBInstanceCount;
	oabbSBDesc.InstanceStride = sizeof( OABBInstance );
	oabbSBDesc.InstanceOffset = 0;

	KVE::Graphics::RenderManager::Instance().createShaderBuffers( oabbSBDesc, 0 );
}