#pragma pack_matrix(row_major)

struct VS_IN
{
	float4 pos : POSITION0;
	float4 tex : TEXCOORD0;
	float4 normal : NORMAL0;
};

struct ObjectData
{
	float4x4 World;
	float4x4 WorldView;
	float4x4 WorldViewProj;
	float4x4 InvTransWorld;
};

cbuffer ObjectBufffer : register(b0)
{
	ObjectData ObjData;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 tex : TEXCOORD;
	float4 normal : NORMAL;

	float4 worldPos : WORLDPOS;
};

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos.xyz, 1.0f), ObjData.WorldViewProj);
	output.normal = mul(float4(input.normal.xyz, 0.0f), ObjData.InvTransWorld);

	output.worldPos = mul(float4(input.pos.xyz, 1.0f), ObjData.World);

	output.tex = input.tex;
	
	return output;
}



struct GBuffer
{
	float4 DiffuseSpec : SV_Target0;
	float3 WorldPos : SV_Target1;
	float3 Normal : SV_Target1;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

[earlydepthstencil]
GBuffer PSMain(PS_IN input) : SV_Target
{
	GBuffer result = (GBuffer)0;
	
	result.DiffuseSpec = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);
	result.WorldPos = input.worldPos.xyz;
	result.Normal = normalize(input.normal.xyz);
	
	return result;
}
