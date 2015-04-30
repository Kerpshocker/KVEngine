#include "Resources.h"

using namespace DirectX;

namespace KVE
{
	std::string* split( std::string s, char delimiter )
	{
		int numDelimitiers = std::count( s.begin(), s.end(), delimiter );

		std::string* temp = new std::string[ numDelimitiers + 1 ];
		
		int j = 0;

		for ( int i = 0; i < s.length() - 1; i++ )
		{
			if ( s[ i ] != delimiter )
			{
				temp[ j ] += s[ i ];
			}
			else
			{
				j++;
			}
		}

		return temp;
	}

	bool createSBDescFromOBJFile( std::string objFilePath, KVE::ShaderBuffersDesc* sbDesc, std::size_t vertices )
	{
		std::string data = "";
		std::string line = "";

		try
		{
			std::ifstream file( objFilePath.c_str() );
			file.exceptions( std::ifstream::failbit );

			while ( !file.eof() )
			{
				std::getline( file, line );
				data += line + '\n';
			}

			file.close();
		}
		catch ( std::ifstream::failure e )
		{
			/*
			// how to catch here?
			std::string errorText = "Could not find " + objFilePath + "\n\n" + e.what() + "\n\n" + e.code().message();

			MessageBox(NULL, std::wstring(errorText.begin(), errorText.end()).c_str(),
			L"File not found error!", MB_ICONEXCLAMATION | MB_OK);
			*/

			return false;
		}

		// remove all comments from file before reading data
		int numComments = std::count( data.begin(), data.end(), '#' );
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
		std::string faces = data.substr( data.find_first_of( 'f' ), data.length() );
		faces = faces.substr( 0, faces.find_last_of( "0123456789" ) + 1 ) + '\n';
		data = data.substr( 0, data.length() - faces.length() );
		data = data.substr( 0, data.find_last_of( "0123456789" ) + 1 ) + '\n';

		int Normals = std::count( data.begin(), data.end(), 'n' );
		int UVs = std::count( data.begin(), data.end(), 't' );
		int Positions = std::count( data.begin(), data.end(), 'v' ) - Normals - UVs;

		int iVerts = 0; XMFLOAT3* tempVerts = ( Positions > 0 ) ? new XMFLOAT3[ Positions ] : nullptr;
		int iNorms = 0; XMFLOAT3* tempNorms = ( Normals > 0 ) ? new XMFLOAT3[ Normals ] : nullptr;
		int iUVs = 0; XMFLOAT2* tempUVs = ( UVs > 0 ) ? new XMFLOAT2[ UVs ] : nullptr;
		//int iColrs = 0; XMFLOAT4* tempColrs = ( Colors > 0 ) ? new XMFLOAT4[ Colors ] : nullptr;

		std::string* s = nullptr;

		while ( data.length() > 0 )
		{
			s = nullptr;
			line = data.substr( 0, data.find_first_of( '\n' ) );
			data = data.substr( line.length() + 1, data.length() );

			if ( line[ 0 ] == 'v' && line[ 1 ] != 't' && line[ 1 ] != 'n' && line[ 1 ] != 'c' ) // v = Vertex
			{
				line = line.substr( 2, line.length() - 1 );
				s = split( line, ' ' );

				tempVerts[ iVerts++ ] = XMFLOAT3( (float)atof( s[ 0 ].c_str() ),
					(float)atof( s[ 1 ].c_str() ),
					(float)atof( s[ 2 ].c_str() ) );
			}
			else if ( line[ 0 ] == 'v' && line[ 1 ] == 'n' ) // vn = Vertex Normal
			{
				line = line.substr( 3, line.length() - 1 );
				s = split( line, ' ' );

				tempNorms[ iNorms++ ] = XMFLOAT3( (float)atof( s[ 0 ].c_str() ),
					(float)atof( s[ 1 ].c_str() ),
					(float)atof( s[ 2 ].c_str() ) );
			}
			else if ( line[ 0 ] == 'v' && line[ 1 ] == 't' ) // vt = Vertex Texture (UV)
			{
				line = line.substr( 3, line.length() - 1 );
				s = split( line, ' ' );

				tempUVs[ iUVs++ ] = XMFLOAT2( (float)atof( s[ 0 ].c_str() ),
					(float)atof( s[ 1 ].c_str() ) );
			}
			//else if ( line[ 0 ] == 'v' && line[ 1 ] == 'c' ) // vc = Vertex Color
			//{
			//	line = line.substr( 3, line.length() - 1 );
			//	s = split( line, ' ' );

			//	tempColrs[ iColrs++ ] = XMFLOAT4( (float)atof( s[ 0 ].c_str() ),
			//		(float)atof( s[ 1 ].c_str() ),
			//		(float)atof( s[ 2 ].c_str() ),
			//		(float)atof( s[ 3 ].c_str() ) );
			//}
			else
			{
				// we done fucked up
			}

			delete[] s;
			s = nullptr;
		}

		int numIndices = std::count( faces.begin(), faces.end(), 'f' ) * 3; // 3 vertices in each face
		
		//delete when adding normals and uvs of the obj
		Normals = 0;
		UVs = 0;
		int byteSize =	( Positions != 0 ) * sizeof( DirectX::XMFLOAT3 ) +
						( Normals != 0 ) * sizeof( DirectX::XMFLOAT3 ) +
						( UVs != 0 ) * sizeof( DirectX::XMFLOAT2 );

		void* vertexData = malloc( numIndices *  byteSize);
		int tail = 0;

		while ( faces.length() > 0 )
		{
			line = faces.substr( 0, faces.find_first_of( '\n' ) );
			faces = faces.substr( line.length() + 1, faces.length() );

			if ( line[ 0 ] == 'f' )
			{
				line = line.substr( 2, line.length() - 1 ); // 2 for "f "
				s = split( line, ' ' );

				std::string* s2 = nullptr;
				int indices = std::count( line.begin(), line.end(), ' ' ) + 1;
				for ( int i = 0; i < indices; i++ )
				{
					int numData = std::count( s[ i ].begin(), s[ i ].end(), '/' );
					s2 = split( s[ i ], '/' );

					*(XMFLOAT3*)( (byte*)vertexData + tail ) = tempVerts[ atoi( s2[ 0 ].c_str() ) - 1 ];
					tail += sizeof( XMFLOAT3 );

					//if ( s2[ 2 ].length() > 0 ) // if there is a normal
					//{
					//	*(XMFLOAT3*)( (byte*)vertexData + tail ) = tempNorms[ atoi( s2[ 2 ].c_str() ) - 1 ];
					//	tail += sizeof( XMFLOAT3 );
					//}

					//if ( s2[ 1 ].length() > 0 ) // if there is a uv
					//{
					//	*(XMFLOAT2*)( (byte*)vertexData + tail ) = tempUVs[ atoi( s2[ 1 ].c_str() ) - 1 ];
					//	tail += sizeof( XMFLOAT2 );
					//}

					//if ( numData > 2 && s2[ 3 ].length() > 0 ) // if there is a color
					//{
					//	*(XMFLOAT4*)( (byte*)vertexData + tail ) = tempColrs[ atoi( s2[ 3 ].c_str() ) - 1 ];
					//	tail += sizeof( XMFLOAT4 );
					//}

					delete[] s2;
					s2 = nullptr;
				}
			}
			else
			{
				// we done fucked up again
			}

			delete[] s;
			s = nullptr;
		}

		UINT* indices = new UINT[ numIndices ];
		for ( int i = 0; i < numIndices; i++ )
		{
			indices[ i ] = i;
		}

		sbDesc->Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		sbDesc->Vertices = vertexData;
		sbDesc->VertexCount = iVerts;
		sbDesc->VertexOffset = 0;
		sbDesc->VertexStride = vertices;
		sbDesc->VertexIndexCount = numIndices;
		sbDesc->VertexIndices = indices;

		delete[] tempVerts;
		delete[] tempNorms;
		delete[] tempUVs;
		//delete[] tempColrs;

		return true;
	}
}