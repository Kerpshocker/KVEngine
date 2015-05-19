#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <Windows.h>
#include "Manager.h"
#include "DXWindow.h"
#include <DirectXMath.h>
#include "Spaceship.h"
#include "OBB.h"
#include "GameTimer.h"

struct Vertex
{
	DirectX::XMFLOAT3	Position;
	DirectX::XMFLOAT3	Normal;
};

struct MeshInstance
{
	DirectX::XMFLOAT3	Position;
	DirectX::XMFLOAT4	Color;
};

struct OABBInstance
{
	DirectX::XMFLOAT3	Position;
	DirectX::XMFLOAT4	Color;
};

struct ObjectData
{
	DirectX::XMVECTOR	dir;
	f32					velocity;
};

struct FrameParams;

class GameManager : public KVE::Utilities::Manager
{
	SINGLETON_INSTANCE( GameManager );
public:
	void initialize( const KVE::Graphics::DXWindow* window, const KVE::System::GameTimer* timer );
	void release( void ) { }
	void update( void );

private:
	void createShaders( void );
	void createGeometry( void );
	void collides( ObjectData* instance, ObjectData* other, DirectX::XMVECTOR instancePos, DirectX::XMVECTOR otherPos );

	const KVE::System::GameTimer*	m_Timer;
	KVE::Graphics::FrameParams*		m_CurrentFrame;
	double							m_LastFrameEndTime;

	KVE::Collisions::OBB*			m_OBB; 

	UINT							m_InstanceCount;
	MeshInstance*					m_MeshInstances;
	OABBInstance*					m_OABBInstances;

	ObjectData*						m_ObjectData;

	Spaceship						m_Player;
};

#endif