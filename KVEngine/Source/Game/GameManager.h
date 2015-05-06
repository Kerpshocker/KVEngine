#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <Windows.h>
#include "Manager.h"
#include "DXWindow.h"
#include <DirectXMath.h>
#include "Spaceship.h"
#include "GameTimer.h"

struct Vertex
{
	DirectX::XMFLOAT3 Position;
};

struct MeshInstance
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
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

	const KVE::System::GameTimer*	m_Timer;

	KVE::Graphics::FrameParams*		m_CurrentFrame;

	MeshInstance*					m_LocalInstances;
	UINT							m_LocalInstanceCount;

	Spaceship						m_Player;
};

#endif