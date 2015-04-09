#include "RenderManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"
#include "Window.h"
#include <d3dcompiler.h>
#include "DXWindow.h"

using namespace DirectX;

void RenderManager::initialize( const DXWindow* window )
{
	KVE::CameraParams cParams;
	cParams.fieldOfView = 45.0f * ( 3.1415f / 180.0f );
	cParams.nearPlane = .01f;
	cParams.farPlane = 100.0f;

	CameraManager::Instance().createNewCamera( cParams, true );
	CameraManager::Instance().getActiveCamera()->setProjMatrix( window->m_AspectRatio );
	CameraManager::Instance().getActiveCamera()->setViewMatrix();

	XMMATRIX world = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4( &worldMatrix, world );

	vsDataToConstantBuffer.World = worldMatrix;
	vsDataToConstantBuffer.Proj = CameraManager::Instance().getActiveCamera()->getProjMatrix();
	vsDataToConstantBuffer.View = CameraManager::Instance().getActiveCamera()->getViewMatrix();

	createGeometry();
	loadShadersAndInputLayout( window->m_Renderer.device );

	// Update the constant buffer itself
	window->m_Renderer.deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,
		NULL,
		&vsDataToConstantBuffer,
		0,
		0 );

	Manager::initialize();
}

void RenderManager::renderTo( const DXWindow* window )
{
	//draw background
	const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f }; //black

	// Clear the buffer
	window->m_Renderer.deviceContext->ClearRenderTargetView( 
		window->m_Renderer.renderTargetView, 
		color );
	window->m_Renderer.deviceContext->ClearDepthStencilView(
		window->m_Renderer.depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0 );

	// Set up the input assembler
	window->m_Renderer.deviceContext->IASetInputLayout( inputLayout );
	window->m_Renderer.deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Set buffers in the input assembler
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	window->m_Renderer.deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
	window->m_Renderer.deviceContext->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
	window->m_Renderer.deviceContext->VSSetShader( vertexShader, NULL, 0 );
	window->m_Renderer.deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&vsConstantBuffer );
	window->m_Renderer.deviceContext->PSSetShader( pixelShader, NULL, 0 );

	// Finally do the actual drawing
	window->m_Renderer.deviceContext->DrawIndexed(
		3,	// The number of indices we're using in this draw
		0,
		0 );

	// Present the buffer
	HR( window->m_Renderer.swapChain->Present( 0, 0 ) );
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

void RenderManager::loadShadersAndInputLayout( ID3D11Device* device )
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