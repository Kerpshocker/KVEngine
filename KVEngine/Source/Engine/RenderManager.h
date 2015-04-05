#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Singleton.h"
#include "dxerr.h"

#include <string>
#include <d3d11.h>
#include <assert.h>

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)												\
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

class RenderManager
{
	SINGLETON_INSTANCE( RenderManager );
protected:
	std::wstring windowCaption;
	int windowWidth;
	int windowHeight;
	bool enable4xMsaa;
	UINT msaa4xQuality;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;

public:
	void initialize( void );
	void render( void );
	void onResize( void );

};

#endif