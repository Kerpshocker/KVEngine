#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include <d3d11.h>
#include "dxerr.h"
#include "Manager.h"

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			DXTrace(__FILEW__, (DWORD)__LINE__, hr, L#x, true);	\
			PostQuitMessage(0);									\
		}														\
	}														
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif

struct ShaderProgram
{
	ID3D11VertexShader*			VertexShader;
	ID3D11PixelShader*			PixelShader;
	ID3D11InputLayout*			InputLayout;
};


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
	void*						Vertices;
	UINT						VertexCount;
	UINT						VertexStride;
	UINT						VertexOffset;
	void*						Indices;
	UINT						IndexCount;
	void*						ConstBufferData;
	UINT						ConstBufferByteSize;
	D3D_PRIMITIVE_TOPOLOGY		Topology;
};

class DXWindow;

class RenderManager : public Manager
{
	SINGLETON_INSTANCE( RenderManager );
public:
	void initialize( const D3D11_VIEWPORT* viewports, const UINT numViewports );
	void release( void );
	void render( void );

	void setWindow( const DXWindow* const window );

	void createShaderProgram( const ShaderProgramDesc& spDesc );
	void createShaderBuffers( const ShaderBuffersDesc& sbDesc );

private:
	const DXWindow* m_Window;
	
	D3D11_VIEWPORT* m_Viewports;
	UINT m_ViewportCount;

	ShaderProgram m_ShaderProgram;
	ShaderBuffers m_ShaderBuffers;

};

#endif