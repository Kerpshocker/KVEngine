#include "DXWindow.h"
#include "RenderManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace KVE
{
	namespace Graphics
	{
		void RenderManager::initialize( const D3D11_VIEWPORT* viewports, const UINT numViewports )
		{
			m_ViewportCount = numViewports;
			m_Viewports = new D3D11_VIEWPORT[ m_ViewportCount ];
			memcpy( m_Viewports, viewports, sizeof( D3D11_VIEWPORT ) * m_ViewportCount );

			Manager::initialize();
		}

		void RenderManager::release( void )
		{
			if ( m_Viewports != nullptr )
			{
				delete[] m_Viewports;
				m_Viewports = nullptr;
			}

			if ( m_ShaderLayouts != nullptr )
			{
				for ( int i = 0; i < 1; i++ )
					m_ShaderLayouts[ i ].Release();

				delete m_ShaderLayouts;
				m_ShaderLayouts = nullptr;
			}

			ReleaseMacro( m_ConstBuffer );
		}

		void RenderManager::setWindow( const DXWindow* const window )
		{
			m_Window = window;

			window->m_DeviceContext->RSSetViewports( m_ViewportCount, m_Viewports );

			createConstBuffer( sizeof( CameraBuffer ) );
		}

		void RenderManager::render( void )
		{
			// make sure there is a window to draw to
			assert( m_Window );

			if ( FrameManager::Instance().isEmpty() ) return;

			FrameManager::Instance().readNextFrame( &m_CurrentFrame );

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

			// set camera constant buffer
			CameraBuffer ccBuffer =
			{
				m_CurrentFrame->ViewMatrix,
				m_CurrentFrame->ProjMatrix,
				m_CurrentFrame->WorldMatrix
			};
			setConstBuffer( &ccBuffer );

			for ( UINT i = 0; i < m_LayoutCount; i++ )
			{
				// Set up the input assembler and set the current vertex and pixel shaders
				m_Window->m_DeviceContext->IASetInputLayout( m_ShaderLayouts[ i ].Program.InputLayout );
				m_Window->m_DeviceContext->VSSetShader( m_ShaderLayouts[ i ].Program.VertexShader, NULL, 0 );
				m_Window->m_DeviceContext->PSSetShader( m_ShaderLayouts[ i ].Program.PixelShader, NULL, 0 );

				for ( UINT j = 0; j < m_ShaderLayouts[ i ].NumBuffers; j++ )
				{
					m_Window->m_DeviceContext->IASetPrimitiveTopology( m_ShaderLayouts[ i ].Buffers[ j ].Mesh.Topology );
					m_Window->m_DeviceContext->IASetVertexBuffers(
						0,
						1,
						&m_ShaderLayouts[ i ].Buffers[ j ].Mesh.VertexBuffer,
						&m_ShaderLayouts[ i ].Buffers[ j ].Mesh.VertexStride,
						&m_ShaderLayouts[ i ].Buffers[ j ].Mesh.VertexOffset
						);
					m_Window->m_DeviceContext->IASetIndexBuffer(
						m_ShaderLayouts[ i ].Buffers[ j ].Mesh.VertexIndexBuffer,
						DXGI_FORMAT_R32_UINT,
						0
						);

					setInstanceBuffer(
						m_CurrentFrame,
						m_ShaderLayouts[ i ].Buffers[ j ].InstanceBuffer,
						m_CurrentFrame->InstanceStride * m_CurrentFrame->InstanceCount,
						i,
						j
						);
					m_Window->m_DeviceContext->IASetVertexBuffers(
						1,
						1,
						&m_ShaderLayouts[ i ].Buffers[ j ].InstanceBuffer,
						&m_ShaderLayouts[ i ].Buffers[ j ].InstanceStride,
						&m_ShaderLayouts[ i ].Buffers[ j ].InstanceOffset
						);

					m_Window->m_DeviceContext->DrawIndexedInstanced(
						m_ShaderLayouts[ i ].Buffers[ j ].Mesh.VertexIndexCount,
						m_ShaderLayouts[ i ].Buffers[ j ].InstanceCount,
						0,
						0,
						0
						);
				}
			}

			// Present the buffer
			HR( m_Window->m_SwapChain->Present( 0, 0 ) );
		}

		UINT RenderManager::createShaderLayout( const ShaderProgramDesc& spDesc )
		{
			assert( m_Window );

			m_ShaderLayouts = new ShaderLayout[ 1 ];

			m_ShaderLayouts[ 0 ].NumBuffers = 0;

			// Load Vertex Shader --------------------------------------
			ID3DBlob* vsBlob;
			D3DReadFileToBlob( spDesc.VShaderFile, &vsBlob );

			// Create the shader on the device
			HR( m_Window->m_Device->CreateVertexShader(
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				NULL,
				&m_ShaderLayouts[ 0 ].Program.VertexShader ) );

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
				return m_LayoutCount++;
			}
		}

		void RenderManager::createShaderBuffers( const ShaderBuffersDesc& sbDesc, UINT layoutIndex )
		{
			assert( m_Window );

			m_ShaderLayouts[ layoutIndex ].Buffers = new ShaderBuffers();

			UINT buffNum = m_ShaderLayouts[ layoutIndex ].NumBuffers++;

			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.VertexStride = sbDesc.VertexStride;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.VertexOffset = sbDesc.VertexOffset;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.VertexIndexCount = sbDesc.VertexIndexCount;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceCount = sbDesc.InstanceCount;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceStride = sbDesc.InstanceStride;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceOffset = sbDesc.InstanceOffset;
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.Topology = sbDesc.Topology;

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
				&m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.VertexBuffer ) );

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
				&m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].Mesh.VertexIndexBuffer ) );

			// Create the instance buffer
			D3D11_BUFFER_DESC instbd;
			instbd.Usage = D3D11_USAGE_DYNAMIC;
			instbd.ByteWidth = sbDesc.InstanceStride * sbDesc.InstanceCount;
			instbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			instbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			instbd.MiscFlags = 0;
			instbd.StructureByteStride = 0;
			HR( m_Window->m_Device->CreateBuffer(
				&instbd,
				0,
				&m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceBuffer ) );

			/*D3D11_MAPPED_SUBRESOURCE mappedInstanceData;

			m_Window->m_DeviceContext->Map(
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedInstanceData
			);
			memcpy( mappedInstanceData.pData, sbDesc.Instances, sbDesc.InstanceStride * sbDesc.InstanceCount );
			m_Window->m_DeviceContext->Unmap(
			m_ShaderLayouts[ layoutIndex ].Buffers[ buffNum ].InstanceBuffer,
			0
			);*/
		}

		void RenderManager::createConstBuffer( const UINT stride )
		{
			// Create the constant buffer
			D3D11_BUFFER_DESC cBufferDesc;
			cBufferDesc.ByteWidth = stride;
			cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cBufferDesc.CPUAccessFlags = 0;
			cBufferDesc.MiscFlags = 0;
			cBufferDesc.StructureByteStride = 0;
			HR( m_Window->m_Device->CreateBuffer(
				&cBufferDesc,
				NULL,
				&m_ConstBuffer ) );

			m_Window->m_DeviceContext->VSSetConstantBuffers(
				0,
				1,
				&m_ConstBuffer
				);
		}

		void RenderManager::setConstBuffer( void* data )
		{
			m_Window->m_DeviceContext->UpdateSubresource(
				m_ConstBuffer,
				0,
				NULL,
				data,
				0,
				0
				);
		}

		void RenderManager::setInstanceBuffer( const FrameParams* frame, ID3D11Buffer* iBuffer, const UINT byteSize, const UINT layoutIndex, const UINT bufferIndex )
		{
			D3D11_MAPPED_SUBRESOURCE mappedInstanceData;

			// maps the underlying InstanceBuffer's data to the local D3D11_MAPPED_SUBRESOURCE 'pData' variable
			m_Window->m_DeviceContext->Map(
				m_ShaderLayouts[ layoutIndex ].Buffers[ bufferIndex ].InstanceBuffer,
				0,
				D3D11_MAP_WRITE_DISCARD, // discard any instance data that was previously here (i.e. in this 'InstanceBuffer')
				0,
				&mappedInstanceData
				);
			// fill 'pData' with the new instance data taken from the top FrameParam in the frames list
			// because the local D3D11_MAPPED_SUBRESOURCE is mapped to the InstanceBuffer data this fills this buffer's data
			memcpy( mappedInstanceData.pData, frame->Instances, frame->InstanceStride * frame->InstanceCount );
			m_Window->m_DeviceContext->Unmap(
				m_ShaderLayouts[ layoutIndex ].Buffers[ bufferIndex ].InstanceBuffer,
				0
				);
		}
	}
}