#pragma pack_matrix(row_major)

#ifndef CASCADE_COUNT
#define CASCADE_COUNT 4
#endif

#define MAX_LIGHTS 8

// Light types.
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_IN VSMain(uint id: SV_VertexID)
{
	PS_IN output = (PS_IN)0;

	output.tex = float2(id & 1, (id & 2) >> 1);
	output.pos = float4(output.tex * float2(2, -2) + float2(-1, 1), 0, 1);

	return output;
}


struct Light
{
	float4      Position;               // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4      Direction;              // 16 bytes
	//----------------------------------- (16 byte boundary)
	float4      Color;                  // 16 bytes
	//----------------------------------- (16 byte boundary)
	float       SpotAngle;              // 4 bytes
	float       ConstantAttenuation;    // 4 bytes
	float       LinearAttenuation;      // 4 bytes
	float       QuadraticAttenuation;   // 4 bytes
	//----------------------------------- (16 byte boundary)
	int         LightType;              // 4 bytes
	bool        Enabled;                // 4 bytes
	int2        Padding;                // 8 bytes
	//----------------------------------- (16 byte boundary)
};  // Total:                           // 80 bytes (5 * 16)

//struct Light
//{
//	float4 Direction;
//	float4 Color;
//};

struct LightingData
{
	Light Lights;
	float4x4 ViewMatrix;
	float3 ViewVector;
	float Intensity;
};

cbuffer LightBuffer : register(b1)
{
	LightingData LightsData;
};

struct LightingResult
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
};

float4 GetColor(float4 worldViewPos);
float4 DoDiffuse(Light light, float3 L, float3 N);
float4 DoSpecular(Light light, float3 V, float3 L, float3 N);
LightingResult DoDirectionalLight(Light light, float3 V, float3 N);
LightingResult ComputeLighting(float4 P, float4 WP, float4 WVP, float3 N);

struct CascadeData
{
	float4x4 ViewProj[CASCADE_COUNT + 1];
	float4 Distances;
};

cbuffer CascadeBuffer : register(b2)
{
	CascadeData CascData;
};

float ShadowCalculation(float4 worldPos, float4 worldViewPos, float3 N, float4 lightDirection);

Texture2D<float4>		DiffuseTex			: register(t0);
Texture2D<float3>		WorldPositions		: register(t1);
Texture2D<float3>		Normals				: register(t2);
Texture2DArray			CascadeShadowMap	: register(t3);

SamplerState			Sampler				: register(s0);
SamplerComparisonState	DepthSampler		: register(s1);


float4 PSMain( PS_IN input ) : SV_Target
{
	float4 norm = float4(normalize(Normals.Load(int3(input.pos.xy, 0))), 1.0f);

	float4 worldPos = float4(WorldPositions.Load(int3(input.pos.xy, 0)).xyz, 1.0f);
	float4 worldViewPos = mul(worldPos, LightsData.ViewMatrix);

	LightingResult lightResult = ComputeLighting(input.pos, worldPos, worldViewPos, norm);

	float4 ambient = lightResult.Ambient * LightsData.Intensity; //TODO: multiply to material
	float4 diffuse = lightResult.Diffuse * LightsData.Intensity; //TODO: multiply to material
	float4 specular = lightResult.Specular * LightsData.Intensity; //TODO: multiply to material

	//texture color
	float4 objColor = float4(DiffuseTex.Load(int3(input.pos.xy, 0)).xyz, 1.0f);
	//float4 objColor = GetColor(worldViewPos);

	float4 result = (ambient + (diffuse + specular)) * objColor;
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

float DoAttenuation(Light light, float d)
{
	return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation * d + light.QuadraticAttenuation * d * d);
}

LightingResult DoPointLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);

	result.Ambient = float4(0, 0, 0, 0);
	result.Diffuse = DoDiffuse(light, L, N) * attenuation;
	result.Specular = DoSpecular(light, V, L, N) * attenuation;

	return result;
}

LightingResult DoDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = -light.Direction.xyz;

	result.Ambient = light.Color;
	result.Diffuse = DoDiffuse(light, L, N);
	result.Specular = DoSpecular(light, V, L, N);

	return result;
}

float DoSpotCone(Light light, float3 L)
{
	float minCos = cos(light.SpotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.Direction.xyz, -L);
	return smoothstep(minCos, maxCos, cosAngle);
}

LightingResult DoSpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = DoAttenuation(light, distance);
	float spotIntensity = DoSpotCone(light, L);

	result.Ambient = float4( 0, 0, 0, 0 );
	result.Diffuse = DoDiffuse(light, L, N) * attenuation * spotIntensity;
	result.Specular = DoSpecular(light, V, L, N) * attenuation * spotIntensity;

	return result;
}

LightingResult ComputeLighting(float4 P, float4 worldPos, float4 worldViewPos, float3 N)
{
	float3 V = LightsData.ViewVector;
	P = worldPos;

	LightingResult totalResult = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };

	/*[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{*/
		LightingResult result = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
		float shadow = 1.0f;

		//if (!LightsData.Lights.Enabled) continue;

		switch (LightsData.Lights.LightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = DoDirectionalLight(LightsData.Lights, V, P, N);
			shadow = 1.0f - ShadowCalculation(worldPos, worldViewPos, N, -LightsData.Lights.Direction);
		}
		break;
		case POINT_LIGHT:
		{
			result = DoPointLight(LightsData.Lights, V, P, N);
			//shadow = 1.0f - ShadowCalculation(worldPos, worldViewPos, N, LightsData.Lights[i].Position - worldPos);
		}
		break;
		case SPOT_LIGHT:
		{
			result = DoSpotLight(LightsData.Lights, V, P, N);
		}
		break;
		}

		result.Diffuse *= shadow;
		result.Specular *= shadow;

		totalResult.Ambient += result.Ambient;
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	//}

	//totalResult = DoDirectionalLight(LightsData.Lights, LightsData.ViewVector, N);

	//totalResult.Diffuse = saturate(totalResult.Diffuse);
	//totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}

float ShadowCalculation(float4 worldPos, float4 worldViewPos, float3 N, float4 lightDirection)
{
	// select cascade layer
	float depthValue = abs(worldViewPos.z);

	int layer = -1;
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		if (depthValue < CascData.Distances[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = CASCADE_COUNT;
	}

	float4 lightViewProjPos = mul(float4(worldPos.xyz, 1.0), CascData.ViewProj[layer]);

	// perform perspective divide
	float3 projCoords = lightViewProjPos.xyz / lightViewProjPos.w;

	// transform to [0,1] range
	projCoords.x = projCoords.x * 0.5f + 0.5f;
	projCoords.y = projCoords.y * -0.5f + 0.5f;

	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0f)
	{
		return 0.0f;
	}

	// calculate bias (based on depth map resolution and slope)
	float bias = max(0.05f * (1.0f - dot(N, lightDirection)), 0.005f);
	const float biasModifier = 0.5f;
	if (layer == CASCADE_COUNT)
	{
		bias *= 1 / (1000.0 * biasModifier);
	}
	else
	{
		bias *= 1 / (CascData.Distances[layer] * biasModifier);
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

	// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
	if (projCoords.z > 1.0)
	{
		shadow = 0.0f;
	}

	return shadow;
}

float4 GetColor(float4 worldViewPos)
{
	float depthValue = abs(worldViewPos.z);

	int layer = -1;
	for (int i = 0; i < CASCADE_COUNT; ++i)
	{
		if (depthValue < CascData.Distances[i])
		{
			layer = i;
			break;
		}
	}
	if (layer == -1)
	{
		layer = CASCADE_COUNT;
	}

	return float4(float3(0.2f, 0.2f, 0.2f) * (layer + 1), 1.0f);
}