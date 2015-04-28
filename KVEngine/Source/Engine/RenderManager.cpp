#include "DXWindow.h"
#include "RenderManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

struct CameraConstBuffer
{
	DirectX::XMFLOAT4X4 ViewMatrix;
	DirectX::XMFLOAT4X4 ProjMatrix;
	DirectX::XMFLOAT4X4 WorldMatrix;
};

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

	window->m_DeviceContext->RSSetViewports(m_ViewportCount, m_Viewports);

	createConstBuffer(sizeof(CameraConstBuffer));
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

	// set camera constant buffer
	KVE::CameraBuffer ccBuffer =
	{
		m_FramesList[ 0 ].ViewMatrix,
		m_FramesList[ 0 ].ProjMatrix,
		m_FramesList[ 0 ].WorldMatrix
	};
	setConstBuffer(&ccBuffer);

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
				m_ShaderLayouts[ i ].Buffers[ j ].InstanceBuffer, 
				m_FramesList[ 0 ].InstanceStride * m_FramesList[ 0 ].InstanceCount,
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

UINT RenderManager::createShaderLayout( const KVE::ShaderProgramDesc& spDesc )
{
	assert( m_Window );

	m_ShaderLayouts = new KVE::ShaderLayout[ 1 ];

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

void RenderManager::createShaderBuffers( const KVE::ShaderBuffersDesc& sbDesc, UINT layoutIndex )
{
	assert( m_Window );

	m_ShaderLayouts[ layoutIndex ].Buffers = new KVE::ShaderBuffers();
	
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

void RenderManager::pushFrame( KVE::FrameParams frame )
{
	m_FramesList[ 0 ] = frame;
}

void RenderManager::setInstanceBuffer( ID3D11Buffer* iBuffer, const UINT byteSize, const UINT layoutIndex, const UINT bufferIndex )
{
	D3D11_MAPPED_SUBRESOURCE mappedInstanceData;

	// maps the underlying InstanceBuffer's data to the local D3D11_MAPPED_SUBRESOURCE 'pData' variable
	m_Window->m_DeviceContext->Map(
		m_ShaderLayouts[layoutIndex].Buffers[bufferIndex].InstanceBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD, // discard any instance data that was previously here (i.e. in this 'InstanceBuffer')
		0,
		&mappedInstanceData
		);
	// fill 'pData' with the new instance data taken from the top FrameParam in the frames list
	// because the local D3D11_MAPPED_SUBRESOURCE is mapped to the InstanceBuffer data this fills this buffer's data
	memcpy( mappedInstanceData.pData, m_FramesList[ 0 ].Instances, m_FramesList[ 0 ].InstanceStride * m_FramesList[ 0 ].InstanceCount );
	m_Window->m_DeviceContext->Unmap(
		m_ShaderLayouts[ layoutIndex ].Buffers[ bufferIndex ].InstanceBuffer,
		0
		);
}

//should return an instance of a new mesh*
void RenderManager::loadMeshFromOBJFile( std::string objFilePath )
{
	std::string data = "";
	std::string line = "";

	try
	{
		std::ifstream file( objFilePath.c_str( ) );
		file.exceptions( std::ifstream::failbit );

		while ( !file.eof( ) )
		{
			std::getline( file, line );
			data += line + '\n';
		}

		file.close( );
	}
	catch ( std::ifstream::failure e )
	{
		/*
		// how to catch here?
		std::string errorText = "Could not find " + objFilePath + "\n\n" + e.what() + "\n\n" + e.code().message();

		MessageBox(NULL, std::wstring(errorText.begin(), errorText.end()).c_str(),
		L"File not found error!", MB_ICONEXCLAMATION | MB_OK);
		*/
	}

	// remove all comments from file before reading data
	int numComments = std::count( data.begin( ), data.end( ), '#' );
	while ( numComments > 0 )
	{
		int comment = data.find_first_of( '#' );

		while ( comment != 0 && data[ comment - 1 ] == ' ' )
			comment--;

		while ( data[ comment ] != '\n' )
			data.erase( comment, 1 );

		numComments--;
	}

	// seperate vertex data from polygon faces
	std::string faces = data.substr( data.find_first_of( 'f' ), data.length( ) );
	faces = faces.substr( 0, faces.find_last_of( "0123456789" ) + 1 ) + '\n';
	data = data.substr( 0, data.length( ) - faces.length( ) );
	data = data.substr( 0, data.find_last_of( "0123456789" ) + 1 ) + '\n';

	VertexHelper vh;
	vh.Normals = std::count( data.begin( ), data.end( ), 'n' );
	vh.UVs = std::count( data.begin( ), data.end( ), 't' );
	vh.Colors = std::count( data.begin( ), data.end( ), 'c' );
	vh.Positions = std::count( data.begin( ), data.end( ), 'v' ) - vh.Normals - vh.UVs - vh.Colors;

	int iVerts = 0; XMFLOAT3* tempVerts = ( vh.Positions > 0 ) ? new XMFLOAT3[ vh.Positions ] : nullptr;
	int iNorms = 0; XMFLOAT3* tempNorms = ( vh.Normals > 0 ) ? new XMFLOAT3[ vh.Normals ] : nullptr;
	int iUVs = 0; XMFLOAT2* tempUVs = ( vh.UVs > 0 ) ? new XMFLOAT2[ vh.UVs ] : nullptr;
	int iColrs = 0; XMFLOAT4* tempColrs = ( vh.Colors > 0 ) ? new XMFLOAT4[ vh.Colors ] : nullptr;

	std::string* split = nullptr;

	while ( data.length( ) > 0 )
	{
		split = nullptr;
		line = data.substr( 0, data.find_first_of( '\n' ) );
		data = data.substr( line.length( ) + 1, data.length( ) );

		if ( line[ 0 ] == 'v' && line[ 1 ] != 't' && line[ 1 ] != 'n' && line[ 1 ] != 'c' ) // v = Vertex
		{
			line = line.substr( 2, line.length( ) - 1 );
			//split = Split( line, ' ' );

			tempVerts[ iVerts++ ] = XMFLOAT3( ( float )atof( split[ 0 ].c_str( ) ),
				( float )atof( split[ 1 ].c_str( ) ),
				( float )atof( split[ 2 ].c_str( ) ) );
		}
		else if ( line[ 0 ] == 'v' && line[ 1 ] == 'n' ) // vn = Vertex Normal
		{
			line = line.substr( 3, line.length( ) - 1 );
			//split = Split( line, ' ' );

			tempNorms[ iNorms++ ] = XMFLOAT3( ( float )atof( split[ 0 ].c_str( ) ),
				( float )atof( split[ 1 ].c_str( ) ),
				( float )atof( split[ 2 ].c_str( ) ) );
		}
		else if ( line[ 0 ] == 'v' && line[ 1 ] == 't' ) // vt = Vertex Texture (UV)
		{
			line = line.substr( 3, line.length( ) - 1 );
			//split = Split( line, ' ' );

			tempUVs[ iUVs++ ] = XMFLOAT2( ( float )atof( split[ 0 ].c_str( ) ),
				( float )atof( split[ 1 ].c_str( ) ) );
		}
		else if ( line[ 0 ] == 'v' && line[ 1 ] == 'c' ) // vc = Vertex Color
		{
			line = line.substr( 3, line.length( ) - 1 );
			//split = Split( line, ' ' );

			tempColrs[ iColrs++ ] = XMFLOAT4( ( float )atof( split[ 0 ].c_str( ) ),
				( float )atof( split[ 1 ].c_str( ) ),
				( float )atof( split[ 2 ].c_str( ) ),
				( float )atof( split[ 3 ].c_str( ) ) );
		}
		else
		{
			// there be problems in them hills
		}

		delete[ ] split;
		split = nullptr;
	}

	int numIndices = std::count( faces.begin( ), faces.end( ), 'f' ) * 3; // 3 vertices in each face
	void* vertexData = malloc( numIndices * vh.GetIndividualBytes( ) );
	int tail = 0;

	while ( faces.length( ) > 0 )
	{
		line = faces.substr( 0, faces.find_first_of( '\n' ) );
		faces = faces.substr( line.length( ) + 1, faces.length( ) );

		if ( line[ 0 ] == 'f' )
		{
			line = line.substr( 2, line.length( ) - 1 ); // 2 for "f "
			//split = Split( line, ' ' );

			std::string* split2 = nullptr;
			int indices = std::count( line.begin( ), line.end( ), ' ' ) + 1;
			for ( int i = 0; i < indices; i++ )
			{
				int numData = std::count( split[ i ].begin( ), split[ i ].end( ), '/' );
				//split2 = Split( split[ i ], '/' );

				*( XMFLOAT3* )( ( byte* )vertexData + tail ) = tempVerts[ atoi( split2[ 0 ].c_str( ) ) - 1 ];
				tail += sizeof( XMFLOAT3 );

				if ( split2[ 2 ].length( ) > 0 ) // if there is a normal
				{
					*( XMFLOAT3* )( ( byte* )vertexData + tail ) = tempNorms[ atoi( split2[ 2 ].c_str( ) ) - 1 ];
					tail += sizeof( XMFLOAT3 );
				}

				if ( split2[ 1 ].length( ) > 0 ) // if there is a uv
				{
					*( XMFLOAT2* )( ( byte* )vertexData + tail ) = tempUVs[ atoi( split2[ 1 ].c_str( ) ) - 1 ];
					tail += sizeof( XMFLOAT2 );
				}

				if ( numData > 2 && split2[ 3 ].length( ) > 0 ) // if there is a color
				{
					*( XMFLOAT4* )( ( byte* )vertexData + tail ) = tempColrs[ atoi( split2[ 3 ].c_str( ) ) - 1 ];
					tail += sizeof( XMFLOAT4 );
				}

				delete[ ] split2;
				split2 = nullptr;
			}
		}
		else
		{
			// there be problems in them hills
		}

		delete[ ] split;
		split = nullptr;
	}

	UINT* indices = new UINT[ numIndices ];
	for ( int i = 0; i < numIndices; i++ )
	{
		indices[ i ] = i;
	}

	/*VertexArray verts;
	verts.data = vertexData;
	verts.TotalArrayBytes = numIndices * vh.GetIndividualBytes( );
	verts.IndividualBytes = vh.GetIndividualBytes( );

	verts.InputLayoutName = "P";
	if ( vh.Normals > 0 ) verts.InputLayoutName += "N";
	if ( vh.UVs > 0 ) verts.InputLayoutName += "U";
	if ( vh.Colors > 0 ) verts.InputLayoutName += "C";*/

	delete[ ] tempVerts;
	delete[ ] tempNorms;
	delete[ ] tempUVs;
	delete[ ] tempColrs;



	//
	//access to vertex and index buffers for the mesh would need to get passed in via an extra parameter I believe...
	//

	//KVE::Mesh* m = new KVE::Mesh;
	//m->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//m->VertexOffset = 0;
	////m->VertexStride = sizeof( VertexArray );
	//m->VertexIndexCount = numIndices;

	//return m;
}