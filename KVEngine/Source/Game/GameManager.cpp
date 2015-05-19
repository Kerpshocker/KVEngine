#include "GameManager.h"
#include "CameraManager.h"
#include "RenderManager.h"
#include "FrameManager.h"
#include "InputManager.h"
#include "GameTimer.h"
#include "MemoryManager.h"
#include <d3dcompiler.h>

using namespace DirectX;

static KVE::System::PageAllocator* gameMemory;

void GameManager::initialize( const KVE::Graphics::DXWindow* window, const KVE::System::GameTimer* timer )
{
	gameMemory = new KVE::System::PageAllocator();

	m_Timer = timer;

	KVE::Graphics::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;
	cParams.initialPos = XMFLOAT3( 0.0f, 0.0f, -5.0f );
	KVE::Graphics::CameraManager::Instance().createNewCamera( cParams, true );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setViewMatrix();

	m_ObjectData = (ObjectData*)gameMemory->alloc( sizeof( ObjectData ) * 3 );
	m_ObjectData[ 0 ] = { { 1.0f, 1.0f, 1.0f }, 0.5f, 0.8f };
	m_ObjectData[ 1 ] = { { 1.0f, 1.0f, 1.0f }, 0.5f, 0.8f };
	m_ObjectData[ 2 ] = { { 1.0f, 1.0f, 1.0f }, 0.5f, 0.8f };

	createShaders();
	createGeometry();

	Manager::initialize();
}

void GameManager::release( void )
{
	if ( gameMemory != nullptr )
	{
		delete gameMemory;
		gameMemory = nullptr;
	}
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

	m_CurrentFrame->Allocator->reset();
	m_CurrentFrame->Instances = m_CurrentFrame->Allocator->alloc(
		sizeof( MeshInstance ) * m_InstanceCount +
		sizeof( OABBInstance ) * m_InstanceCount
		);
	m_CurrentFrame->InstanceStrides[ 0 ] = sizeof( MeshInstance );
	m_CurrentFrame->InstanceStrides[ 1 ] = sizeof( OABBInstance );
	m_CurrentFrame->InstanceCounts[ 0 ] = m_InstanceCount;
	m_CurrentFrame->InstanceCounts[ 1 ] = m_InstanceCount;
	m_CurrentFrame->InstanceOffsets[ 0 ] = 0;
	m_CurrentFrame->InstanceOffsets[ 1 ] = sizeof( MeshInstance ) * m_InstanceCount;

	MeshInstance* frameMeshInstances = (MeshInstance*)m_CurrentFrame->Instances;
	OABBInstance* frameOABBInstances = (OABBInstance*)&frameMeshInstances[ m_InstanceCount ];

	m_MeshInstances[ 0 ].Position.x += m_ObjectData[ 0 ].velocity * (f32)m_CurrentFrame->DeltaTime;

	bool iValue;
	for ( UINT i = 0; i < m_InstanceCount; i++ )
	{
		frameMeshInstances[ i ] = m_MeshInstances[ i ];

		frameOABBInstances[ i ].Position = m_MeshInstances[ i ].Position;
		frameOABBInstances[ i ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );

		/*for ( UINT j = 0; j < i; j++ )
		{
		iValue = m_OBB->intersects( XMLoadFloat3( &frameMeshInstances[ i ].Position ), XMLoadFloat3( &frameMeshInstances[ j ].Position ) );
		if ( iValue == KVE::Collisions::INTERSECTS )
		{
		m_ObjectData[ i ].velocity = 0.0f;
		m_ObjectData[ j ].velocity = 0.0f;
		frameOABBInstances[ i ].Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		frameOABBInstances[ j ].Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		}
		else
		{
		frameOABBInstances[ i ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		frameOABBInstances[ j ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
		}
		}*/
	}

	/*KVE::Collisions::IntersectionValue iValue;*/
	for ( UINT i = 0; i < m_InstanceCount; i++ )
	{
		for ( UINT j = i + 1; j < m_InstanceCount - 1; j++ )
		{
			iValue = m_OBB->intersects( XMLoadFloat3( &frameMeshInstances[ i ].Position ), XMLoadFloat3( &frameMeshInstances[ j ].Position ) );

			if ( iValue )
			{
				m_ObjectData[ i ].velocity = 0.0f;
				m_ObjectData[ j ].velocity = 0.0f;
				frameOABBInstances[ i ].Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
				frameOABBInstances[ j ].Color = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
			}
			else
			{
				frameOABBInstances[ i ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
				frameOABBInstances[ j ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
			}
		}
	}

	m_LastFrameEndTime = m_CurrentFrame->EndTime = m_Timer->totalTime();

	KVE::Graphics::FrameManager::Instance().closeFrame( &m_CurrentFrame );
}

void GameManager::createShaders( void )
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		// vertex buffer
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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
	m_InstanceCount = 3;

	// Set up the mesh instances
	m_MeshInstances = (MeshInstance*)gameMemory->alloc( sizeof( MeshInstance ) * m_InstanceCount );
	// Position, Color
	m_MeshInstances[ 0 ] = { XMFLOAT3( -1.5f, -1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	m_MeshInstances[ 1 ] = { XMFLOAT3( +1.5f, -1.0f, 0.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) };
	m_MeshInstances[ 2 ] = { XMFLOAT3( 0.0f, +1.0f, 0.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) };

	// Create shader buffers - vertices, indices, and instances
	KVE::Graphics::ShaderBuffersDesc meshSBDesc;
	KVE::Graphics::createSBDescFromOBJFile( "asteroid_obj.obj", &meshSBDesc, sizeof( Vertex ) );
	meshSBDesc.InstanceCount = m_InstanceCount;
	meshSBDesc.InstanceStride = sizeof( MeshInstance );
	meshSBDesc.InstanceOffset = 0;
	meshSBDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	KVE::Graphics::RenderManager::Instance().createShaderBuffers( meshSBDesc, 0 );
	delete[] meshSBDesc.Vertices;
	delete[] meshSBDesc.VertexIndices;

	// Set up the obb instances
	m_OBB = ( KVE::Collisions::OBB* )gameMemory->alloc( sizeof( KVE::Collisions::OBB ) );
	m_OABBInstances = (OABBInstance*)gameMemory->alloc( sizeof( OABBInstance ) * m_InstanceCount );

	XMVECTOR frontTopRight = XMVectorSet( 0.5f, 0.5f, -0.5f, 0.0f );
	XMVECTOR backBottomLeft = XMVectorSet( -0.5f, -0.5f, 0.5f, 0.0f );
	*m_OBB = KVE::Collisions::OBB( XMLoadFloat3( &m_MeshInstances[ 0 ].Position ), frontTopRight, backBottomLeft );

	XMVECTOR* obbCorners = m_OBB->getCollisionCorners();
	Vertex corners[ 8 ];
	for ( int i = 0; i < 8; i++ )
	{
		corners[ i ].Position = { XMVectorGetX( obbCorners[ i ] ), XMVectorGetY( obbCorners[ i ] ), XMVectorGetZ( obbCorners[ i ] ) };
		XMStoreFloat3( &corners[ i ].Normal, XMVector3Normalize( XMLoadFloat3( &corners[ i ].Position ) ) );
	}

	KVE::Graphics::ShaderBuffersDesc oabbSBDesc;
	oabbSBDesc.Topology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	oabbSBDesc.Vertices = corners;
	oabbSBDesc.VertexCount = 8;
	oabbSBDesc.VertexOffset = 0;
	oabbSBDesc.VertexStride = sizeof( Vertex );
	oabbSBDesc.VertexIndexCount = 24;
	oabbSBDesc.VertexIndices = m_OBB->getIndices();
	oabbSBDesc.InstanceCount = m_InstanceCount;
	oabbSBDesc.InstanceStride = sizeof( OABBInstance );
	oabbSBDesc.InstanceOffset = 0;
	KVE::Graphics::RenderManager::Instance().createShaderBuffers( oabbSBDesc, 0 );
}