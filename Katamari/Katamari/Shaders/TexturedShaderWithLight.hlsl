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

struct Light
{
	float4 Direction;
	float4 Color;
};

cbuffer LightProperties : register(b1)
{
	Light Lights;
	float3 ViewVector;
	float Intensity;
};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

float4 DoDiffuse(Light light, float3 L, float3 N);
float4 DoSpecular(Light light, float3 V, float3 L, float3 N);

LightingResult DoDirectionalLight(Light light, float3 V, float3 N);
LightingResult ComputeLighting(float4 P, float3 N);


Texture2D		DiffuseMap		: register(t0);
SamplerState	Sampler			: register(s0);


PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;

	output.pos = mul(float4(input.pos.xyz, 1.0f), WorldViewProj);
	output.normal = mul(float4(input.normal.xyz, 0.0f), InvTransWorld);

	output.tex = input.tex;

	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 norm = normalize(input.normal);
	LightingResult lightResult = ComputeLighting(input.pos, norm);

	float4 ambient = Lights.Color; //TODO: multiply to material
	float4 diffuse = lightResult.Diffuse * Intensity; //TODO: multiply to material
	float4 specular = lightResult.Specular * Intensity; //TODO: multiply to material

	//texture color
	float4 objColor = DiffuseMap.SampleLevel(Sampler, input.tex.xy, 0);

	float4 result = (ambient + diffuse + specular) * objColor;
	return result;
}

float4 DoDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 DoSpecular(Light light, float3 V, float3 L, float3 N)
{
	// Phong lighting.
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	// Blinn-Phong lighting
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.Color * pow(RdotV, 32); //TODO: change power to material
}

LightingResult DoDirectionalLight(Light light, float3 V, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);

	return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
	//float3 V = normalize(EyePosition - P).xyz;

	LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0} };
	totalResult = DoDirectionalLight(Lights, ViewVector, N);

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}
