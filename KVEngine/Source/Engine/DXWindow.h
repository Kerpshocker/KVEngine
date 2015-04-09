#ifndef DX_WINDOW_H
#define DX_WINDOW_H

#include <d3d11.h>
#include "Window.h"
#include "RenderManager.h"

class DXWindow : public Window
{
	friend class RenderManager;
public:
	DXWindow( const WindowParams& params );

private:
	struct Renderer
	{
		bool					enable4xMsaa;
		UINT					msaa4xQuality;

		ID3D11Device*			device;
		ID3D11DeviceContext*	deviceContext;
		IDXGISwapChain*			swapChain;
		ID3D11Texture2D*		depthStencilBuffer;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RenderTargetView* renderTargetView;
	} m_Renderer;

	void onResize( void );

};

#endif