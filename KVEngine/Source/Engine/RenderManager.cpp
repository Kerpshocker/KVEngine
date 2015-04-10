#include "DXWindow.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include <d3dcompiler.h>

using namespace DirectX;

void RenderManager::initialize( const D3D11_VIEWPORT* viewports, const UINT numViewports )
{
	m_ViewportCount = numViewports;
	m_Viewports = new D3D11_VIEWPORT[ m_ViewportCount ];
	memcpy( m_Viewports, viewports, sizeof( D3D11_VIEWPORT ) * m_ViewportCount );

	Manager::initialize();
}

void RenderManager::release( void )
{
	ReleaseMacro( m_ShaderProgram.VertexShader );
	ReleaseMacro( m_ShaderProgram.InputLayout );
	ReleaseMacro( m_ShaderProgram.PixelShader );

	ReleaseMacro( m_ShaderBuffers.VertexBuffer );
	ReleaseMacro( m_ShaderBuffers.IndexBuffer );
	ReleaseMacro( m_ShaderBuffers.ConstantBuffer );

	if ( m_Viewports != nullptr )
	{
		delete[] m_Viewports;
		m_Viewports = nullptr;
	}
}

void RenderManager::setWindow( const DXWindow* const window )
{
	m_Window = window;
	
	window->m_DeviceContext->RSSetViewports( m_ViewportCount, m_Viewports );
}

void RenderManager::render( void )
{
	// make sure there is a window to draw to
	assert( m_Window );

	// draw background
	const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f }; // black

	// Clear the buffer
	m_Window->m_DeviceContext->ClearRenderTargetView(
		m_Window->m_RenderTargetView,
		color );
	m_Window->m_DeviceContext->ClearDepthStencilView(
		m_Window->m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0 );

	// FOR EACH SHADER PROGRAM
	// Set up the input assembler and set the current vertex and pixel shaders
	m_Window->m_DeviceContext->IASetInputLayout( m_ShaderProgram.InputLayout );
	m_Window->m_DeviceContext->VSSetShader( m_ShaderProgram.VertexShader, NULL, 0 );
	m_Window->m_DeviceContext->PSSetShader( m_ShaderProgram.PixelShader, NULL, 0 );

	// FOR EACH SHADER BUFFERS
	m_Window->m_DeviceContext->IASetPrimitiveTopology( m_ShaderBuffers.Topology );
	m_Window->m_DeviceContext->IASetVertexBuffers(
		0,
		1,
		&m_ShaderBuffers.VertexBuffer,
		&m_ShaderBuffers.VertexStride,
		&m_ShaderBuffers.VertexOffset
		);
	m_Window->m_DeviceContext->IASetIndexBuffer(
		m_ShaderBuffers.IndexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
		);

	m_Window->m_DeviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_ShaderBuffers.ConstantBuffer
		);

	m_Window->m_DeviceContext->DrawIndexed(
		m_ShaderBuffers.IndexCount,
		0,
		0
		);
	// END FOR EACH SHADER BUFFERS
	// END FOR EACH SHADER PROGRAM

	// Present the buffer
	HR( m_Window->m_SwapChain->Present( 0, 0 ) );
}

void RenderManager::createShaderProgram( const ShaderProgramDesc& spDesc )
{
	assert( m_Window );

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob( spDesc.VShaderFile, &vsBlob );

	// Create the shader on the device
	HR( m_Window->m_Device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_ShaderProgram.VertexShader ) );

	// Before cleaning up the data, create the input layout
	HR( m_Window->m_Device->CreateInputLayout(
		spDesc.VertexDesc,
		spDesc.NumVertexElements,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_ShaderProgram.InputLayout ) );

	// Clean up
	ReleaseMacro( vsBlob );

	// Load Pixel Shader --------------------------------------
	ID3DBlob *psBlob;
	D3DReadFileToBlob( spDesc.PShaderFile, &psBlob );

	// Create the shader on the device
	HR( m_Window->m_Device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&m_ShaderProgram.PixelShader ) );

	// Clean up
	ReleaseMacro( psBlob );
}

void RenderManager::createShaderBuffers( const ShaderBuffersDesc& sbDesc )
{
	assert( m_Window );

	m_ShaderBuffers.VertexStride = sbDesc.VertexStride;
	m_ShaderBuffers.VertexOffset = sbDesc.VertexOffset;
	m_ShaderBuffers.IndexCount   = sbDesc.IndexCount;
	m_ShaderBuffers.Topology	 = sbDesc.Topology;

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sbDesc.VertexStride * sbDesc.VertexCount; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = sbDesc.Vertices;
	HR( m_Window->m_Device->CreateBuffer( 
		&vbd, 
		&initialVertexData, 
		&m_ShaderBuffers.VertexBuffer ) );

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT ) * sbDesc.IndexCount; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = sbDesc.Indices;
	HR( m_Window->m_Device->CreateBuffer( 
		&ibd, 
		&initialIndexData, 
		&m_ShaderBuffers.IndexBuffer ) );

	// Create the constant buffer
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = sbDesc.ConstBufferByteSize;
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialCBufferData;
	initialCBufferData.pSysMem = sbDesc.ConstBufferData;
	HR( m_Window->m_Device->CreateBuffer( 
		&cBufferDesc, 
		&initialCBufferData, 
		&m_ShaderBuffers.ConstantBuffer ) );
}