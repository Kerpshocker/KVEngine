struct VertexToPixel
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float3 normal	: TEXCOORD;

	float3 worldPos	: POSITION;
};

float4 main( VertexToPixel input ) : SV_TARGET
{
	float3 lightDir = float3( 0.0f, 0.0f, 1.0f );

	input.normal = normalize( input.normal );

	return input.color * nDotL;
}