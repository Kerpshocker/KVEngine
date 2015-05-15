#ifndef RESOURCES_H
#define RESOURCES_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

namespace KVE
{
	namespace Graphics
	{
		struct CameraBuffer
		{
			DirectX::XMFLOAT4X4			ViewMatrix;
			DirectX::XMFLOAT4X4			ProjMatrix;
			DirectX::XMFLOAT4X4			WorldMatrix;
		};

		struct Mesh
		{
			UINT padding;
			D3D_PRIMITIVE_TOPOLOGY		Topology;
			UINT						VertexStride;
			UINT						VertexOffset;
			UINT						VertexIndexCount;
			ID3D11Buffer*				VertexBuffer;
			ID3D11Buffer*				VertexIndexBuffer;

			void Release( void )
			{
				ReleaseMacro( VertexBuffer );
				ReleaseMacro( VertexIndexBuffer );
			}
		};

		struct Material
		{
			ID3D11ShaderResourceView*	ShaderResourceView;
			ID3D11SamplerState*			SamplerState;

			void Release( void )
			{
				ReleaseMacro( ShaderResourceView );
				ReleaseMacro( SamplerState );
			}
		};

		struct ShaderProgram
		{
			ID3D11VertexShader*			VertexShader;
			ID3D11InputLayout*			InputLayout;
			ID3D11PixelShader*			PixelShader;

			void Release( void )
			{
				ReleaseMacro( VertexShader );
				ReleaseMacro( InputLayout );
				ReleaseMacro( PixelShader );
			}
		};

		struct ShaderProgramDesc
		{
			LPCWSTR						VShaderFile;
			LPCWSTR						PShaderFile;
			UINT						NumVertexElements;
			D3D11_INPUT_ELEMENT_DESC*	InputDesc;
		};

		struct ShaderBuffers
		{
			Mesh						Mesh;
			Material					Material;
			// instances

			// SHOULD NOT BE HERE
			UINT						InstanceCount;
			UINT						InstanceStride;
			UINT						InstanceOffset;
			ID3D11Buffer*				InstanceBuffer;

			void Release( void )
			{
				Mesh.Release();
				Material.Release();
				ReleaseMacro( InstanceBuffer );
			}
		};

		struct ShaderBuffersDesc
		{
			D3D_PRIMITIVE_TOPOLOGY		Topology;
			UINT						VertexCount;
			UINT						VertexStride;
			UINT						VertexOffset;
			UINT						VertexIndexCount;
			void*						Vertices;
			void*						VertexIndices;

			// SHOULD NOT BE HERE
			UINT						InstanceCount;
			UINT						InstanceStride;
			UINT						InstanceOffset;
		};

		struct ShaderLayout
		{
			UINT BufferCount;
			UINT BufferMaxCount;
			ShaderProgram Program;
			ShaderBuffers** Buffers;

			void Release( void )
			{
				Program.Release();

				if ( Buffers != nullptr )
				{
					for ( UINT i = 0; i < BufferCount; i++ )
						Buffers[ i ]->Release();

					//delete[] Buffers;
					Buffers = nullptr;
				}
			}
		};

		std::string* split( std::string s, char delimiter );
		bool createSBDescFromOBJFile( std::string objFilePath, ShaderBuffersDesc* sbDesc, std::size_t vertices );
	}
}

#endif