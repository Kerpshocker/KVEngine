#pragma once

//remove this header when we include our main dxgame file
#include <d3d11.h>

class Material
{
public:
	ID3D11ShaderResourceView *srv;
	ID3D11SamplerState *ss;

public:
	Material();
	Material( ID3D11ShaderResourceView *shaderResourceView, ID3D11SamplerState *samplerState );
	~Material();
};


