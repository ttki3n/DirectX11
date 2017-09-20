cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

struct a2v
{
	float3 position: POSITION;
	float2 tex: TEXCOORD0;
	float3 normal: NORMAL;
};

struct v2p
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
	float3 normal: NORMAL;
};

v2p LightVertexShader(a2v input)
{
	v2p output;
	
	output.position = mul(float4(input.position, 1.0f), world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)world);

	// normalize the normal vector
	output.normal = normalize(output.normal);
	return output;
}