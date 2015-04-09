#include "DXWindow.h"
#include <assert.h>

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

DXWindow::DXWindow( const WindowParams& params )
	: Window( params )
{
	m_Renderer.enable4xMsaa = false;
	m_Renderer.msaa4xQuality = 0;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = m_Width;
	swapChainDesc.BufferDesc.Height = m_Height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_HWindow;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	if ( m_Renderer.enable4xMsaa )
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_Renderer.msaa4xQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	UINT createDeviceFlags = 0;
#if defined(DEBUG) | defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;
	HRESULT hr = 0;
	hr = D3D11CreateDeviceAndSwapChain(
		0,
		driverType,
		0,
		createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_Renderer.swapChain,
		&m_Renderer.device,
		&featureLevel,
		&m_Renderer.deviceContext
		);
	if ( FAILED( hr ) )
	{
		MessageBox( 0, L"D3D11CreateDevice Failed", 0, 0 );
		return;
	}

	hr = m_Renderer.device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m_Renderer.msaa4xQuality
		);
	assert( m_Renderer.msaa4xQuality > 0 );

	onResize();
}

void DXWindow::onResize( void )
{
	ReleaseMacro( m_Renderer.renderTargetView );
	ReleaseMacro( m_Renderer.depthStencilView );
	ReleaseMacro( m_Renderer.depthStencilBuffer );

	m_Renderer.swapChain->ResizeBuffers(
		1,
		m_Width,
		m_Height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0
	);
	ID3D11Texture2D* backBuffer;
	HR( m_Renderer.swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &backBuffer ) ) );
	HR( m_Renderer.device->CreateRenderTargetView( backBuffer, 0, &m_Renderer.renderTargetView ) );
	ReleaseMacro( backBuffer );

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	if ( m_Renderer.enable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_Renderer.msaa4xQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	HR( m_Renderer.device->CreateTexture2D( &depthStencilDesc, 0, &m_Renderer.depthStencilBuffer ) );
	HR( m_Renderer.device->CreateDepthStencilView( m_Renderer.depthStencilBuffer, 0, &m_Renderer.depthStencilView ) );

	m_Renderer.deviceContext->OMSetRenderTargets( 1, &m_Renderer.renderTargetView, m_Renderer.depthStencilView );

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_Width;
	viewport.Height = m_Height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_Renderer.deviceContext->RSSetViewports( 1, &viewport );

	Window::onResize( m_Width, m_Height );
}