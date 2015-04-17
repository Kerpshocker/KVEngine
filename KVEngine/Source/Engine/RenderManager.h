#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>
#include "dxerr.h"
#include "Manager.h"
#include "Resources.h"

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

class DXWindow;

class RenderManager : public Manager
{
	SINGLETON_INSTANCE( RenderManager );
public:
	void initialize( const D3D11_VIEWPORT* viewports, const UINT numViewports );
	void release( void );
	void render( void );

	void setWindow( const DXWindow* const window );

	void createShaderProgram( const KVE::ShaderProgramDesc& spDesc );
    void createShaderBuffers( const KVE::ShaderBuffersDesc& sbDesc );

private:
	const DXWindow* m_Window;
	
	D3D11_VIEWPORT* m_Viewports;
	UINT m_ViewportCount;

	KVE::ShaderLayout* m_ShaderLayouts;

	//KVE::ShaderProgram m_ShaderProgram;
	//KVE::ShaderBuffers* m_ShaderBuffers;

};

#endif