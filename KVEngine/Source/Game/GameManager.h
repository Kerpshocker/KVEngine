#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H

#include <Windows.h>
#include "Manager.h"
#include "DXWindow.h"
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

struct VSDataToConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
};

class GameManager : public Manager
{
	SINGLETON_INSTANCE( GameManager );
public:
	void initialize( const DXWindow* window );
	void update( void );

private:
	void createShaders( const DXWindow* window );
	void createGeometry( const DXWindow* window );

	ShaderProgram m_ShaderProgram;
	ShaderBuffers m_ShaderBuffers;

};

#endif