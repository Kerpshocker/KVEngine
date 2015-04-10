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
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;
	ID3D11InputLayout* InputLayout;
	D3D_PRIMITIVE_TOPOLOGY Topology;
};

struct ShaderBuffers
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;
	ID3D11Buffer*	ConstantBuffer;
	UINT			VertexStride;
	UINT			VertexOffset;
	UINT			IndexCount;
};

class DXWindow;

class RenderManager : public Manager
{
	SINGLETON_INSTANCE( RenderManager );
public:
	void initialize( void );
	void renderTo( const DXWindow* window );

	void setShaderProgram( ShaderProgram* shaderProgram );
	void setShaderBuffers( ShaderBuffers* shaderBuffers );

private:
	ShaderProgram* m_ShaderProgram;
	ShaderBuffers* m_ShaderBuffers;

};

#endif