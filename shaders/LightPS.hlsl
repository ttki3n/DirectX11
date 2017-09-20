Texture2D shaderTexture;
SamplerState sampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};
struct v2p
{
	float4 position: SV_POSITION;
	float2 tex: TEXCOORD0;
	float3 normal: NORMAL;
};

float4 LightPixelShader(v2p input): SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location
	textureColor = shaderTexture.Sample(sampleType, input.tex);

	// Invert the light direction for calculations
	lightDir = -lightDirection;

	// Calculate the amount of light on this pixel
	lightIntensity = saturate(dot(input.normal, lightDir));

	// Determine the final amount of diffuse color 
	color = saturate(diffuseColor * lightIntensity);

	// Multiply the texture pixel and the final diffuse color to get the final pixel color result
	color = color * textureColor;

	return color;
}