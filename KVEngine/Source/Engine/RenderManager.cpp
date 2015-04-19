#include "DXWindow.h"
#include "RenderManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

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
	ReleaseMacro( m_ShaderLayouts );

	/*ReleaseMacro( m_ShaderProgram.VertexShader );
	ReleaseMacro( m_ShaderProgram.InputLayout );
	ReleaseMacro( m_ShaderProgram.PixelShader );

	ReleaseMacro( m_ShaderBuffers );*/

	//ReleaseMacro( m_ShaderBuffers.VertexBuffer );
	//ReleaseMacro( m_ShaderBuffers.IndexBuffer );
	//ReleaseMacro( m_ShaderBuffers.ConstantBuffer );

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

	// FOR EACH SHADER LAYOUT
    for ( UINT i = 0; i < m_LayoutCount; i++ )
    {
        // Set up the input assembler and set the current vertex and pixel shaders
        m_Window->m_DeviceContext->IASetInputLayout( m_ShaderLayouts[ i ].Program.InputLayout );
        m_Window->m_DeviceContext->VSSetShader( m_ShaderLayouts[ i ].Program.VertexShader, NULL, 0 );
        m_Window->m_DeviceContext->PSSetShader( m_ShaderLayouts[ i ].Program.PixelShader, NULL, 0 );
    }

    //WHY WOULD THIS BE A DIFFERENT FOR LOOP? 
	// FOR EACH SHADER BUFFER UNDER CURRENT PROGRAM
	m_Window->m_DeviceContext->IASetPrimitiveTopology( m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.Topology );
	m_Window->m_DeviceContext->IASetVertexBuffers(
		0,
		1,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.VertexBuffer,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.VertexStride,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.VertexOffset
		);
	m_Window->m_DeviceContext->IASetVertexBuffers(
		1,
		1,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].InstanceBuffer,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].InstanceStride,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].InstanceOffset
		);
	m_Window->m_DeviceContext->IASetIndexBuffer(
		m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.VertexIndexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
		);

	m_Window->m_DeviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_ShaderLayouts[ 0 ].Buffers[ 0 ].ConstBuffer
		);

	m_Window->m_DeviceContext->DrawIndexedInstanced(
		m_ShaderLayouts[ 0 ].Buffers[ 0 ].Mesh.VertexIndexCount,
		m_ShaderLayouts[ 0 ].Buffers[ 0 ].InstanceCount,
		0,
		0,
		0
		);
	// END FOR EACH SHADER BUFFERS
	// END FOR EACH SHADER PROGRAM

	// Present the buffer
	HR( m_Window->m_SwapChain->Present( 0, 0 ) );
}

UINT RenderManager::createShaderLayout( const KVE::ShaderProgramDesc& spDesc )
{
	assert( m_Window );

	m_ShaderLayouts = new KVE::ShaderLayout[ 1 ];

	// Load Vertex Shader --------------------------------------
	ID3DBlob* vsBlob;
	D3DReadFileToBlob( spDesc.VShaderFile, &vsBlob );

	// Create the shader on the device
	HR( m_Window->m_Device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&m_ShaderLayouts[0].Program.VertexShader ) );

	// Before cleaning up the data, create the input layout
	HR( m_Window->m_Device->CreateInputLayout(
		spDesc.InputDesc,
		spDesc.NumVertexElements,
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&m_ShaderLayouts[ 0 ].Program.InputLayout ) );

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
		&m_ShaderLayouts[ 0 ].Program.PixelShader ) );

	// Clean up
	ReleaseMacro( psBlob );

    if ( m_LayoutCount >= MAX_LAYOUTS )
    {
        //max num of layouts reached
        return -1;
    }
    else
    {
        m_LayoutCount++;
    }

    return m_LayoutCount;
}

void RenderManager::createShaderBuffers( const KVE::ShaderBuffersDesc& sbDesc, UINT layoutCount )
{
	assert( m_Window );

    for ( UINT i = 0; i < layoutCount; i++ )
    {
        m_ShaderLayouts[ i ].Buffers = new KVE::ShaderBuffers[ 1 ];

        m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.VertexStride = sbDesc.VertexStride;
        m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.VertexOffset = sbDesc.VertexOffset;
        m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.VertexIndexCount = sbDesc.VertexIndexCount;
        m_ShaderLayouts[ i ].Buffers[ 0 ].InstanceCount = sbDesc.InstanceCount;
        m_ShaderLayouts[ i ].Buffers[ 0 ].InstanceStride = sbDesc.InstanceStride;
        m_ShaderLayouts[ i ].Buffers[ 0 ].InstanceOffset = sbDesc.InstanceOffset;
        m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.Topology = sbDesc.Topology;

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
            &m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.VertexBuffer ) );

        // Create the index buffer
        D3D11_BUFFER_DESC ibd;
        ibd.Usage = D3D11_USAGE_IMMUTABLE;
        ibd.ByteWidth = sizeof( UINT )* sbDesc.VertexIndexCount; // Number of indices in the "model" you want to draw
        ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ibd.CPUAccessFlags = 0;
        ibd.MiscFlags = 0;
        ibd.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA initialIndexData;
        initialIndexData.pSysMem = sbDesc.VertexIndices;
        HR( m_Window->m_Device->CreateBuffer(
            &ibd,
            &initialIndexData,
            &m_ShaderLayouts[ i ].Buffers[ 0 ].Mesh.VertexIndexBuffer ) );

        // Create the instance buffer
        D3D11_BUFFER_DESC instbd;
        instbd.Usage = D3D11_USAGE_IMMUTABLE;
        instbd.ByteWidth = sbDesc.InstanceStride * sbDesc.InstanceCount;
        instbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        instbd.CPUAccessFlags = 0;
        instbd.MiscFlags = 0;
        instbd.StructureByteStride = 0;
        D3D11_SUBRESOURCE_DATA initialInstanceData;
        initialInstanceData.pSysMem = sbDesc.Instances;
        HR( m_Window->m_Device->CreateBuffer(
            &instbd,
            &initialInstanceData,
            &m_ShaderLayouts[ i ].Buffers[ 0 ].InstanceBuffer ) );

        // Create the constant buffer
        D3D11_BUFFER_DESC cBufferDesc;
        cBufferDesc.ByteWidth = sbDesc.ConstBufferStride;
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
            &m_ShaderLayouts[ i ].Buffers[ 0 ].ConstBuffer ) );
    }
}