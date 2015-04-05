#include "Material.h"


Material::Material()
{
}

Material::Material( ID3D11ShaderResourceView *shaderResourceView, ID3D11SamplerState *samplerState )
{
	ss = samplerState;
	srv = shaderResourceView;
}

Material::~Material()
{
}
