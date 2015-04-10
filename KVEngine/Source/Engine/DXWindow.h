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
	~DXWindow( void );

	//ID3D11Device*			m_Device; // REMOVE THIS
	//ID3D11DeviceContext*	m_DeviceContext; // REMOVE THIS

private:
	bool					m_Enable4xMsaa;
	UINT					m_Msaa4xQuality;

	ID3D11Device*			m_Device;
	ID3D11DeviceContext*	m_DeviceContext;
	IDXGISwapChain*			m_SwapChain;
	ID3D11Texture2D*		m_DepthStencilBuffer;
	ID3D11DepthStencilView* m_DepthStencilView;
	ID3D11RenderTargetView* m_RenderTargetView;

	void onResize( void );

};

#endif