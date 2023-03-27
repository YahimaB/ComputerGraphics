#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

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
	float4 viewPos : VIEWPOS;
	float4 worldPos : WORLDPOS;
};

cbuffer ConstBuf : register(b0) {
	float4x4 WorldViewProj;
	float4x4 World;
	float4x4 WorldView;
	float4x4 InvTransWorld;
}

cbuffer cbCascade : register(b2)
{
	float4x4 gViewProj[CASCADE_COUNT + 1];
	float4 gDistances;
};

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
	float4x4 gT;
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
float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN);

Texture2D				DiffuseMap			: register(t0);
Texture2DArray			CascadeShadowMap	: register(t1);

SamplerState			Sampler				: register(s0);
SamplerComparisonState	DepthSampler		: register(s1);

PS_IN VSMain( VS_IN input )
{
	PS_IN output = (PS_IN)0;

	output.pos = mul(float4(input.pos.xyz, 1.0f), WorldViewProj);
	output.normal = mul(float4(input.normal.xyz, 0.0f), InvTransWorld);
	output.viewPos = mul(float4(input.pos.xyz, 1.0f), WorldView);
	output.worldPos = mul(float4(input.pos.xyz, 1.0f), World);

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

	float shadow = ShadowCalculation(input.worldPos, input.viewPos, dot(norm, -Lights.Direction));

	float4 result = (ambient + (1.0f - shadow) * (diffuse + specular)) * objColor;
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

	//totalResult.Diffuse = saturate(totalResult.Diffuse);
	//totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}

float ShadowCalculation(float4 posWorldSpace, float4 posViewSpace, float dotN)
{
	float depthValue = abs(posViewSpace.z);

	int layer = -1;
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		if (depthValue < gDistances[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = CASCADE_COUNT;
	}

	float4 posLightSpace = mul(float4(posWorldSpace.xyz, 1.0), gViewProj[layer]);
	float3 projCoords = posLightSpace.xyz / posLightSpace.w;

	projCoords = (mul(float4(projCoords, 1.0f), gT)).xyz;
	float currentDepth = projCoords.z;

	if (currentDepth > 1.0f)
	{
		return 0.0f;
	}

	float bias = max(0.05f * (1.0f - dotN), 0.005f);
	const float biasModifier = 0.5f;
	if (layer == CASCADE_COUNT)
	{
		bias *= 1 / (1000.0 * biasModifier);
	}
	else
	{
		bias *= 1 / (gDistances[layer] * biasModifier);
	}

	// PCF
	float shadow = 0.0f;
	float2 texelSize = 1.0f / 1024.0f;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			shadow += CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy + float2(x, y) * texelSize, layer), currentDepth - bias);
		}
	}
	shadow /= 9.0f;

	//float shadow = CascadeShadowMap.SampleCmp(DepthSampler, float3(projCoords.xy, layer), currentDepth);

	return shadow;
}