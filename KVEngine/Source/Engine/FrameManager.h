#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

#include <atomic>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Singleton.h"

namespace KVE
{
	namespace Graphics
	{
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
			static const uint8_t MAX_FRAMES = 10;
			static const uint8_t MAX_QUEUED = 2;

			void initialize( void );
			void release( void );
			bool openFrame( FrameParams** out );
			bool closeFrame( FrameParams** in );
			bool readNextFrame( const FrameParams** const out );
			bool isWriteReady( void ) const;
			bool isReadReady( void ) const;

		private:
			FrameParams m_Data[ MAX_FRAMES ];

			std::atomic<uint8_t>	m_ReadIndex;
			std::atomic<uint8_t>	m_WriteIndex;
			std::atomic<bool>		m_Writing;
		};
	}
}

#endif