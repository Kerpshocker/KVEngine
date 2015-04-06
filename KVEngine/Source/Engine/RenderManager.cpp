#include "RenderManager.h"
#include "Window.h"

void RenderManager::initialize( void )
{
	driverType = D3D_DRIVER_TYPE_HARDWARE;

	UINT createDeviceFlags = 0;

	// Do we want a debug device?
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Set up a swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = Window::Instance().getWindowWidth();
	swapChainDesc.BufferDesc.Height = Window::Instance().getWindowHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = Window::Instance().getWindow();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;
	if ( enable4xMsaa )
	{
		// Set up 4x MSAA
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = msaa4xQuality - 1;
	}
	else
	{
		// No MSAA
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// Create the device and swap chain and determine the supported feature level
	featureLevel = D3D_FEATURE_LEVEL_9_1; // Will be overwritten by next line
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		0,
		driverType,
		0,
		createDeviceFlags,
		0,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		&featureLevel,
		&deviceContext
	);

	// Handle any device creation or DirectX version errors
	if ( FAILED( hr ) )
	{
		MessageBox( 0, L"D3D11CreateDevice Failed", 0, 0 );
		return;
	}

	// Check for 4X MSAA quality support
	HR( device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&msaa4xQuality ) );
	assert( msaa4xQuality > 0 ); // Potential problem if quality is 0

	// The remaining steps also need to happen each time the window
	// is resized, so just run the OnResize method
	onResize();

	Manager::initialize();
}

void RenderManager::render( void )
{
	//draw background
	const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f }; //black

	// Clear the buffer
	deviceContext->ClearRenderTargetView( renderTargetView, color );
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0 );

	// Present the buffer
	HR( swapChain->Present( 0, 0 ) );
}

void RenderManager::onResize( void )
{
	// Release the views, since we'll be destroying
	// the corresponding buffers.
	ReleaseMacro( renderTargetView );
	ReleaseMacro( depthStencilView );
	ReleaseMacro( depthStencilBuffer );

	// Resize the swap chain to match the window and
	// recreate the render target view
	HR( swapChain->ResizeBuffers(
		1,
		Window::Instance().getWindowWidth(),
		Window::Instance().getWindowHeight(),
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0 ) );
	ID3D11Texture2D* backBuffer;
	HR( swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &backBuffer ) ) );
	HR( device->CreateRenderTargetView( backBuffer, 0, &renderTargetView ) );
	ReleaseMacro( backBuffer );

	// Set up the description of the texture to use for the
	// depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Window::Instance().getWindowWidth();
	depthStencilDesc.Height = Window::Instance().getWindowHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	if ( enable4xMsaa )
	{
		// Turn on 4x MultiSample Anti Aliasing
		// This must match swap chain MSAA values
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = msaa4xQuality - 1;
	}
	else
	{
		// No MSAA
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	// Create the depth/stencil buffer and corresponding view
	HR( device->CreateTexture2D( &depthStencilDesc, 0, &depthStencilBuffer ) );
	HR( device->CreateDepthStencilView( depthStencilBuffer, 0, &depthStencilView ) );

	// Bind these views to the pipeline, so rendering actually
	// uses the underlying textures
	deviceContext->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );

	// Update the viewport and set it on the device
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)Window::Instance().getWindowWidth();
	viewport.Height = (float)Window::Instance().getWindowHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports( 1, &viewport );
}

//bool RenderManager::isInitialized( void ) const
//{
//	return initialized;
//}