cbuffer perModel : register( b0 )
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VSInput
{
	float3 position : POSITION;
	float4 color	: COLOR;
};

struct VertexToPixel
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

VertexToPixel main( VSInput input )
{
	VertexToPixel output;

	matrix wvp = mul(mul(world, view), proj);
	output.position = mul(float4(input.position, 1.0f), wvp);

	output.color = input.color;

	return output;
}