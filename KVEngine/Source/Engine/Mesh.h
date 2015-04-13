#include <d3d11.h>
#include "DataTypes.h"

namespace KVE
{
    struct ShaderProgramDesc
    {
        D3D11_INPUT_ELEMENT_DESC*	VertexDesc;
        UINT						NumVertexElements;
        LPCWSTR						VShaderFile;
        LPCWSTR						PShaderFile;
    };

    struct ShaderBuffers
    {
        ID3D11Buffer*				VertexBuffer;
        ID3D11Buffer*				IndexBuffer;
        ID3D11Buffer*				ConstantBuffer;
        UINT						VertexStride;
        UINT						VertexOffset;
        UINT						IndexCount;
        D3D_PRIMITIVE_TOPOLOGY		Topology;
    };

    struct ShaderBuffersDesc
    {
        D3D_PRIMITIVE_TOPOLOGY		Topology;
        void*						ConstBufferData;
        void*						Vertices;
        void*						Indices;
        UINT						VertexCount;
        UINT						IndexCount;
        UINT						VertexStride;
        UINT						VertexOffset;
        UINT						ConstBufferByteSize;
    };

    class Mesh
    {
    public:
        ShaderProgramDesc           m_ShaderProgamDesc;
        ShaderBuffers               m_ShaderBuffers;
        ShaderBuffersDesc           m_ShaderBufferDesc;

    public:
        Mesh( void );
        Mesh( const ShaderBuffers& sb, const ShaderBuffersDesc& sbDesc, const ShaderProgramDesc& spDesc );
        ~Mesh();

#pragma region setters
        void setVertexDesc( D3D11_INPUT_ELEMENT_DESC* VertexDesc );
        void setNumVertexElements( UINT NumVertexElements );
        void setVShaderFile( LPCWSTR VShaderFile );
        void setPShaderFile( LPCWSTR PShaderFile );
        void setVertexBuffer( ID3D11Buffer*	VertexBuffer );
        void setIndexBuffer( ID3D11Buffer*	IndexBuffer );
        void setConstBuffer( ID3D11Buffer* ConstBuffer );
        void setTopology( D3D_PRIMITIVE_TOPOLOGY Topology );
        void setConstBufferData( void* ConstBufferData );
        void setVertices( void*	Vertices );
        void setIndices( void*	Indices );
        void setVertexCount( UINT VertexCount );
        void setIndexCount( UINT IndexCount );
        void setVertexStride( UINT VertexStride );
        void setVertexOffset( UINT VertexOffset );
        void setConstBufferByteSize( UINT ConstBufferByteSize );
#pragma endregion

    };
}