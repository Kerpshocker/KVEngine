#include "Mesh.h"

namespace KVE
{
    Mesh::Mesh( void )
    {

    }

    Mesh::Mesh( const ShaderBuffers& sb, const ShaderBuffersDesc& sbDesc, const ShaderProgramDesc& spDesc )
    {
        m_ShaderProgamDesc.VertexDesc           = spDesc.VertexDesc;
        m_ShaderProgamDesc.NumVertexElements    = spDesc.NumVertexElements;
        m_ShaderProgamDesc.VShaderFile          = spDesc.VShaderFile;
        m_ShaderProgamDesc.PShaderFile          = spDesc.PShaderFile;

        m_ShaderBuffers.VertexBuffer            = sb.VertexBuffer;
        m_ShaderBuffers.IndexBuffer             = sb.IndexBuffer;
        m_ShaderBuffers.ConstantBuffer          = sb.ConstantBuffer;
        m_ShaderBuffers.VertexStride            = sb.VertexStride;
        m_ShaderBuffers.VertexOffset            = sb.VertexOffset;
        m_ShaderBuffers.IndexCount              = sb.IndexCount;
        m_ShaderBuffers.Topology                = sb.Topology;

        m_ShaderBufferDesc.Topology             = sbDesc.Topology;
        m_ShaderBufferDesc.ConstBufferData      = sbDesc.ConstBufferData;
        m_ShaderBufferDesc.Vertices             = sbDesc.Vertices;
        m_ShaderBufferDesc.Indices              = sbDesc.Indices;
        m_ShaderBufferDesc.VertexCount          = sbDesc.VertexCount;
        m_ShaderBufferDesc.IndexCount           = sbDesc.IndexCount;
        m_ShaderBufferDesc.VertexStride         = sbDesc.VertexStride;
        m_ShaderBufferDesc.VertexOffset         = sbDesc.VertexOffset;
        m_ShaderBufferDesc.ConstBufferByteSize  = sbDesc.ConstBufferByteSize;
    }

    Mesh::~Mesh()
    {

    }

#pragma region setters
    void Mesh::setVertexBuffer( ID3D11Buffer* VertexBuffer )
    {
        m_ShaderBuffers.VertexBuffer = VertexBuffer;
    }

    void Mesh::setIndexBuffer( ID3D11Buffer* IndexBuffer )
    {
        m_ShaderBuffers.IndexBuffer = IndexBuffer;
    }

    void Mesh::setConstBuffer( ID3D11Buffer* ConstBuffer )
    {
        m_ShaderBuffers.ConstantBuffer = ConstBuffer;
    }

    void Mesh::setVertexDesc( D3D11_INPUT_ELEMENT_DESC* VertexDesc )
    {
        m_ShaderProgamDesc.VertexDesc = VertexDesc;
    }

    void Mesh::setNumVertexElements( UINT NumVertexElements )
    {
        m_ShaderProgamDesc.NumVertexElements = NumVertexElements;
    }

    void Mesh::setVShaderFile( LPCWSTR VShaderFile )
    {
        m_ShaderProgamDesc.VShaderFile = VShaderFile;
    }

    void Mesh::setPShaderFile( LPCWSTR PShaderFile )
    {
        m_ShaderProgamDesc.PShaderFile = PShaderFile;
    }

    void Mesh::setTopology( D3D_PRIMITIVE_TOPOLOGY Topology )
    {
        m_ShaderBuffers.Topology = Topology;
        m_ShaderBufferDesc.Topology = Topology;
    }

    void Mesh::setConstBufferData( void* ConstBufferData )
    {
        m_ShaderBufferDesc.ConstBufferData = ConstBufferData;
    }

    void Mesh::setVertices( void* Vertices )
    {
        m_ShaderBufferDesc.Vertices = Vertices;
    }

    void Mesh::setIndices( void* Indices )
    {
        m_ShaderBufferDesc.Indices = Indices;
    }

    void Mesh::setVertexCount( UINT	VertexCount )
    {
        m_ShaderBufferDesc.VertexCount = VertexCount;
    }

    void Mesh::setIndexCount( UINT IndexCount )
    {
        m_ShaderBuffers.IndexCount = IndexCount;
        m_ShaderBufferDesc.IndexCount = IndexCount;
    }

    void Mesh::setVertexStride( UINT VertexStride )
    {
        m_ShaderBuffers.VertexStride = VertexStride;
        m_ShaderBufferDesc.VertexStride = VertexStride;
    }

    void Mesh::setVertexOffset( UINT VertexOffset )
    {
        m_ShaderBuffers.VertexOffset = VertexOffset;
        m_ShaderBufferDesc.VertexOffset = VertexOffset;
    }

    void Mesh::setConstBufferByteSize( UINT ConstBufferByteSize )
    {
        m_ShaderBufferDesc.ConstBufferByteSize = ConstBufferByteSize;
    }
#pragma endregion
}