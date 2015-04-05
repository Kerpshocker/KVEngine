#include "RenderManager.h"
#include "Window.h"

void RenderManager::initialize( void )
{
	Manager::initialize();
}

void RenderManager::render( void )
{

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