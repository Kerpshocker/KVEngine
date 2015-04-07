cbuffer perModel : register( b0 )
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VSInput
{
	float3 pos : POSITION;
	float4 col : COLOR;
};

struct VertexToPixel
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

VertexToPixel main(VSInput input)
{
	VertexToPixel output;

	matrix wvp = mul(mul(world, view), proj);
	output.pos = mul(float4(input.pos, 1.0f), wvp);

	output.col = input.col;

	return output;
}