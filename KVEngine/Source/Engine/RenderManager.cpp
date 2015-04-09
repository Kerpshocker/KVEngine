#include "RenderManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "Window.h"
#include <d3dcompiler.h>

using namespace DirectX;

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

	KVE::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;

	CameraManager::Instance().createNewCamera( cParams, true );
	CameraManager::Instance().getActiveCamera()->setProjMatrix();
	CameraManager::Instance().getActiveCamera()->setViewMatrix();

	XMMATRIX world = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4( &worldMatrix, world );

	vsDataToConstantBuffer.World = worldMatrix;
	vsDataToConstantBuffer.Proj = CameraManager::Instance().getActiveCamera()->getProjMatrix();
	vsDataToConstantBuffer.View = CameraManager::Instance().getActiveCamera()->getViewMatrix();

	createGeometry();
	loadShadersAndInputLayout();

	// Update the constant buffer itself
	deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,
		NULL,
		&vsDataToConstantBuffer,
		0,
		0 );

	Manager::initialize();
}

void RenderManager::render( void )
{
	//draw background
	const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f }; //black

	// Clear the buffer
	deviceContext->ClearRenderTargetView( renderTargetView, color );
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0 );

	// Set up the input assembler
	deviceContext->IASetInputLayout( inputLayout );
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Set buffers in the input assembler
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
	deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	deviceContext->VSSetShader( vertexShader, NULL, 0 );
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&vsConstantBuffer );
	deviceContext->PSSetShader( pixelShader, NULL, 0 );

	// Finally do the actual drawing
	deviceContext->DrawIndexed(
		3,	// The number of indices we're using in this draw
		0,
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

void RenderManager::createGeometry( void )
{
	XMFLOAT4 red = XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f );
	XMFLOAT4 green = XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f );
	XMFLOAT4 blue = XMFLOAT4( 0.0f, 0.0f, 1.0f, 1.0f );

	// Set up the vertices
	Vertex vertices[] =
	{
		{ XMFLOAT3( +0.0f, +1.0f, +0.0f ), red },
		{ XMFLOAT3( -1.5f, -1.0f, +0.0f ), green },
		{ XMFLOAT3( +1.5f, -1.0f, +0.0f ), blue },
	};
}

void RenderManager::loadShadersAndInputLayout( void )
{
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob( L"PC_VShader.cso", &vsBlob );

	// Create the shader on the device
	HR( device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader ) );

	// Before cleaning up the data, create the input layout
	HR( device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE( vertexDesc ),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout ) );

	// Clean up
	ReleaseMacro( vsBlob );

	// Compile pixel shader shader
	ID3DBlob *psBlob;
	D3DReadFileToBlob( L"PC_PShader.cso", &psBlob );

	// Create the shader on the device
	HR( device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader ) );

	// Clean up
	ReleaseMacro( psBlob );

	printf( "Success\n" );

	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sizeof( VSDataToConstantBuffer );
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	HR( device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer ) );
}