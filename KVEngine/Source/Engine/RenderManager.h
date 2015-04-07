#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H

#include "Manager.h"
#include "dxerr.h"

#include <string>
#include <d3d11.h>
#include <assert.h>
#include <DirectXMath.h>

// Convenience macro for releasing a COM object
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

// Macro for popping up a text box based
// on a failed HRESULT and then quitting (only in debug builds)
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)												\
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

struct VSDataToConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
};

// Vertex struct for triangles
struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

class RenderManager : public Manager
{
	SINGLETON_INSTANCE( RenderManager );
protected:
	bool enable4xMsaa;
	UINT msaa4xQuality;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;

	// Buffers to hold actual geometry
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vsConstantBuffer;
	VSDataToConstantBuffer vsDataToConstantBuffer;

private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

public:
	void initialize( void );
	void render( void );
	void onResize( void );

	void createGeometry(void);
	void loadShadersAndInputLayout( void );
};

#endif