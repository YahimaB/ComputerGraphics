#pragma pack_matrix(row_major)

struct VS_IN
{
	float4 pos : POSITION0;
	float4 tex : TEXCOORD0;
	float4 normal : NORMAL0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
};

cbuffer ConstBuf : register(b0) {
	float4x4 WorldViewProj;
	float4x4 InvTransWorld;
}

cbuffer cbPerScene : register(b1)
{
	float4 lightDir;
	float4 lightColorAmbStr;
	float4 viewDirSpecStr;
};

Texture2D		DiffuseMap		: register(t0);
SamplerState	Sampler			: register(s0);


PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;

	output.pos = mul(float4(input.pos.xyz, 1.0f), WorldViewProj);
	output.tex = input.tex.xy;

	output.normal = mul(float4(input.normal.xyz, 0.0f), InvTransWorld);

	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);

	float4 ambient = lightColorAmbStr.w * float4(lightColorAmbStr.xyz, 1.0f);

	float4 norm = normalize(input.normal);
	float diff = max(dot(norm, lightDir), 0.0f);
	float4 diffuse = diff * float4(lightColorAmbStr.xyz, 1.0f);

	float4 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDirSpecStr.xyz, reflectDir.xyz), 0.0f), 32);
	float4 specular = viewDirSpecStr.w * spec * float4(lightColorAmbStr.xyz, 1.0f);

	float4 result = (ambient + diffuse + specular) * objColor;

	return float4(result.xyz, 1.0f);
}
