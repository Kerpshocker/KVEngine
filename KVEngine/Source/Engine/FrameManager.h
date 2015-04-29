#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <atomic>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Singleton.h"

struct FrameParams
{
	double DeltaTime;
	double StartTime, EndTime;

	DirectX::XMFLOAT4X4 WorldMatrix;
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjMatrix;

	void*	Instances;
	UINT	InstanceStride;
	UINT	InstanceCount;
};

class FrameManager
{
	SINGLETON_INSTANCE( FrameManager );
public:
	static const uint16_t MAX_FRAMES = 10;

	void initialize( void );
	void release( void );
	bool pushFrame( FrameParams& in );
	bool popFrame( FrameParams& out );
	bool isEmpty( void ) const;
	bool isFull( void ) const;

private:
	FrameParams m_Data[ MAX_FRAMES ];

	std::atomic<uint16_t> m_ReadIndex;
	std::atomic<uint16_t> m_WriteIndex;

};

#endif