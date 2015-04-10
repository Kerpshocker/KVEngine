#include "DXWindow.h"
#include "RenderManager.h"
#include "ResourceManager.h"

using namespace DirectX;

void RenderManager::initialize( void )
{
	Manager::initialize();
}

void RenderManager::renderTo( const DXWindow* window )
{
	// draw background
	const f32 color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f }; // black

	// Clear the buffer
	window->m_DeviceContext->ClearRenderTargetView( 
		window->m_RenderTargetView, 
		color );
	window->m_DeviceContext->ClearDepthStencilView(
		window->m_DepthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0 );

	// FOR EACH SHADER PROGRAM
	// Set up the input assembler and set the current vertex and pixel shaders
	window->m_DeviceContext->IASetInputLayout( m_ShaderProgram->InputLayout );
	window->m_DeviceContext->IASetPrimitiveTopology( m_ShaderProgram->Topology );
	window->m_DeviceContext->VSSetShader( m_ShaderProgram->VertexShader, NULL, 0 );
	window->m_DeviceContext->PSSetShader( m_ShaderProgram->PixelShader, NULL, 0 );

	// FOR EACH SHADER BUFFERS
	window->m_DeviceContext->IASetVertexBuffers(
		0,
		1,
		&m_ShaderBuffers->VertexBuffer,
		&m_ShaderBuffers->VertexStride,
		&m_ShaderBuffers->VertexOffset
		);
	window->m_DeviceContext->IASetIndexBuffer(
		m_ShaderBuffers->IndexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
		);

	window->m_DeviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_ShaderBuffers->ConstantBuffer
		);

	window->m_DeviceContext->DrawIndexed(
		m_ShaderBuffers->IndexCount,
		0,
		0
		);
	// END FOR EACH SHADER BUFFERS
	// END FOR EACH SHADER PROGRAM

	// Present the buffer
	HR( window->m_SwapChain->Present( 0, 0 ) );
}

void RenderManager::setShaderProgram( ShaderProgram* shaderProgram )
{
	m_ShaderProgram = shaderProgram;
}

void RenderManager::setShaderBuffers( ShaderBuffers* shaderBuffers )
{
	m_ShaderBuffers = shaderBuffers;
}