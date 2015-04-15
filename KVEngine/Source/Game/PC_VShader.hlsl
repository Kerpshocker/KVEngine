cbuffer perModel : register( b0 )
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VSInput
{
	// vertex
	float3 position		: POSITION0;

	// instance
	float3 instancePos	: POSITION1;
	float4 instanceCol	: COLOR0;
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
	output.position = mul(float4(input.position + input.instancePos, 1.0f), wvp);

	output.color = input.instanceCol;

	return output;
}