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

	f32 randNum;

	m_InstanceCount = 18;

	KVE::Graphics::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;
	cParams.initialPos = XMFLOAT3( 0.0f, 0.0f, -5.0f );
	KVE::Graphics::CameraManager::Instance().createNewCamera( cParams, true );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setProjMatrix( window->aspectRatio() );
	KVE::Graphics::CameraManager::Instance().getActiveCamera()->setViewMatrix();

	m_ObjectData = (ObjectData*)gameMemory->alloc( sizeof( ObjectData ) * m_InstanceCount );

	for ( int i = 0; i < m_InstanceCount; i++ )
	{
		m_ObjectData[ i ] = { 
			XMLoadFloat3( &XMFLOAT3(
				(f32)( rand() % 100 ) / 99.0f,
				(f32)( rand() % 100 ) / 99.0f,
				(f32)( rand() % 100 ) / 99.0f
			) ),
			randNum = (f32)( rand() % 100 ) / 70.0f != 0 ? (f32)( rand() % 100 ) / 70.0f : 2.0f,
		};
	}

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

	for ( UINT i = 0; i < m_InstanceCount; i++ )
	{
		XMFLOAT3 newPos;
		XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
			XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
			XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
			XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
			) ) );

		if ( m_MeshInstances[ i ].Position.x >= 50.0f )
		{
			m_MeshInstances[ i ].Position.x = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		if ( m_MeshInstances[ i ].Position.x <= -50.0f )
		{
			m_MeshInstances[ i ].Position.x = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		if ( m_MeshInstances[ i ].Position.y >= 50.0f )
		{
			m_MeshInstances[ i ].Position.y = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		if ( m_MeshInstances[ i ].Position.y <= -50.0f )
		{
			m_MeshInstances[ i ].Position.y = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		if ( m_MeshInstances[ i ].Position.z >= 50.0f )
		{
			m_MeshInstances[ i ].Position.z = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		if ( m_MeshInstances[ i ].Position.z <= -50.0f )
		{
			m_MeshInstances[ i ].Position.z = (f32)( rand() % 10 - 5 );
			XMStoreFloat3( &newPos, XMLoadFloat3( &XMFLOAT3(
				XMVectorGetX( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetY( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime ),
				XMVectorGetZ( m_ObjectData[ i ].dir ) * ( m_ObjectData[ i ].velocity * (f32)m_CurrentFrame->DeltaTime )
				) ) );
		}

		m_MeshInstances[ i ].Position.x += newPos.x;
		m_MeshInstances[ i ].Position.y += newPos.y;
		m_MeshInstances[ i ].Position.z += newPos.z;

		frameMeshInstances[ i ] = m_MeshInstances[ i ];

		frameOABBInstances[ i ].Position = m_MeshInstances[ i ].Position;
		frameOABBInstances[ i ].Color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	}

	for ( UINT i = 0; i < m_InstanceCount; i++ )
	{
		for ( UINT j = i + 1; j < m_InstanceCount; j++ )
		{
			bool iValue = m_OBB->intersects( XMLoadFloat3( &frameOABBInstances[ i ].Position ), XMLoadFloat3( &frameOABBInstances[ j ].Position ) );

			if ( iValue )
				collides( &m_ObjectData[ i ], &m_ObjectData[ j ], XMLoadFloat3( &frameOABBInstances[ i ].Position ), XMLoadFloat3( &frameOABBInstances[ j ].Position ) );
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
	// Set up the mesh instances
	m_MeshInstances = (MeshInstance*)gameMemory->alloc( sizeof( MeshInstance ) * m_InstanceCount );
	// Position, Color
	for ( int i = 0; i < m_InstanceCount; i++ )
	{
		m_MeshInstances[ i ] = { 
			XMFLOAT3(
				(f32)( rand() % 10 - 5 ),
				(f32)( rand() % 10 - 5 ),
				(f32)( rand() % 10 - 5 ) 
			), 
			XMFLOAT4( 
				1.0f,
				1.0f,
				1.0f,
				1.0f
			) 
		};
	}

	/*m_MeshInstances[ 0 ] = { XMFLOAT3( -1.5f, -1.0f, 0.0f ), XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ) };
	m_MeshInstances[ 1 ] = { XMFLOAT3( +1.5f, -1.0f, 0.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) };
	m_MeshInstances[ 2 ] = { XMFLOAT3( 0.0f, +1.0f, 0.0f ), XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f ) };*/

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
	*m_OBB = KVE::Collisions::OBB( frontTopRight, backBottomLeft );

	XMVECTOR* obbCorners = m_OBB->getCollisionCorners();
	Vertex corners[ 8 ];
	for ( int i = 0; i < 8; i++ )
	{
		corners[ i ].Position = { XMVectorGetX( obbCorners[ i ] ), XMVectorGetY( obbCorners[ i ] ), XMVectorGetZ( obbCorners[ i ] ) };
		XMStoreFloat3( &corners[ i ].Normal, XMVector3Normalize( XMLoadFloat3( &corners[ i ].Position ) ) );
	}
}

void GameManager::collides( ObjectData* instance, ObjectData* other, DirectX::XMVECTOR instancePos, DirectX::XMVECTOR otherPos )
{
	XMVECTOR iVelDir = XMLoadFloat3( &XMFLOAT3(
		XMVectorGetX( instancePos ) - XMVectorGetX( otherPos ),
		XMVectorGetY( instancePos ) - XMVectorGetY( otherPos ),
		XMVectorGetZ( instancePos ) - XMVectorGetZ( otherPos )
	) );

	instance->dir += iVelDir;
	XMVector3Normalize( instance->dir );

	other->dir += XMVectorNegate( iVelDir );
	XMVector3Normalize( other->dir );
}