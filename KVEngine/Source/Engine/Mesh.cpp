#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

//uncomment the HR function call whenever we include the DXGame
void Mesh::createMesh( ID3D11Device *device, Vertex vertices[], UINT indices[], int numIndices )
{
	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof( Vertex )* numIndices; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;
	//HR( device->CreateBuffer( &vbd, &initialVertexData, &vBuffer ) );

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof( UINT )* numIndices; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	//HR( device->CreateBuffer( &ibd, &initialIndexData, &iBuffer ) );

	indicesPerObject = numIndices;
}

