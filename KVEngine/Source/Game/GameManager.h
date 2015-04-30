#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <Windows.h>
#include "Manager.h"
#include "DXWindow.h"
#include <DirectXMath.h>

class GameTimer;

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

class GameManager : public Manager
{
	SINGLETON_INSTANCE( GameManager );
public:
	void initialize( const DXWindow* window, const GameTimer* timer );
	void release( void ) { }
	void update( void );

private:
	void createShaders( void );
	void createGeometry( void );

	const GameTimer*	m_Timer;

	FrameParams*		m_CurrentFrame;

	MeshInstance*		m_LocalInstances;
	UINT				m_LocalInstanceCount;
};

#endif