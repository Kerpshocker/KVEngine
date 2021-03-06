#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>

#include "dxerr.h"
#include "Manager.h"
#include "Resources.h"
#include "FrameManager.h"

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}														
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

#define MAX_LAYOUTS 2

namespace KVE
{
	namespace Graphics
	{
		class DXWindow;
		struct FrameParams;

		class RenderManager : public Utilities::Manager
		{
			SINGLETON_INSTANCE( RenderManager );
		public:
			void initialize( const UINT numShaderLayouts, const D3D11_VIEWPORT* viewports, const UINT numViewports );
			void release( void );
			void render( void );

			UINT createShaderLayout( const ShaderProgramDesc& spDesc, const UINT bufferCount );
			void createShaderBuffers( const ShaderBuffersDesc& sbDesc, const UINT layoutIndex );

			void setWindow( const DXWindow* const window );
			void setViewport( const D3D11_VIEWPORT* viewport, UINT index );

		private:
			const DXWindow* m_Window;

			UINT			m_ViewportCount;
			D3D11_VIEWPORT* m_Viewports;

			UINT			m_ShaderLayoutCount;
			UINT			m_ShaderLayoutMaxCount;
			ShaderLayout**	m_ShaderLayouts;
			ID3D11Buffer*	m_ConstBuffer;

			FrameParams*	m_CurrentFrame;

			void createConstBuffer( const UINT stride );
			void setConstBuffer( void* data );
			void setInstanceBuffer( const FrameParams* frame, ID3D11Buffer* iBuffer, const UINT byteSize, const UINT layoutIndex, const UINT bufferIndex );
		};
	}
}

#endif