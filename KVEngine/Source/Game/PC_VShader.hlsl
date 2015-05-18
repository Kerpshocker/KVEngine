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
	float3 normal		: TEXCOORD0;

	// instance
	float3 instancePos	: POSITION1;
	float4 instanceCol	: COLOR0;
};

struct VertexToPixel
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float3 normal	: TEXCOORD;

	float3 worldPos	: POSITION;
};

VertexToPixel main( VSInput input )
{
	VertexToPixel output;

	matrix wvp = mul( mul( world, view ), proj );
	output.position = mul( float4( input.position + input.instancePos, 1.0f ), wvp );
	output.worldPos = mul( float4( input.position + input.instancePos, 1.0f ), world ).xyz;

	//output.normal = mul( input.normal, ( float3x3 )world );
	output.normal = normalize( input.normal );

	output.color = input.instanceCol;

	return output;
}