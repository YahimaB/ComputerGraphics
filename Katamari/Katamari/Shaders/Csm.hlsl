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

struct ObjectData
{
	float4x4 World;
};

cbuffer ObjectBufffer : register(b0)
{
	ObjectData ObjData;
};

struct GS_IN
{
	float4 pos : POSITION;
};

GS_IN VSMain(VS_IN input)
{
	GS_IN output = (GS_IN)0;

	output.pos = mul(float4(input.pos.xyz, 1.0f), ObjData.World);

	return output;
}



struct GS_OUT
{
	float4 pos : SV_POSITION;
 	uint arrInd : SV_RenderTargetArrayIndex;
};

struct CascadeData
{
	float4x4 ViewProj[CASCADE_COUNT + 1];
	float4 Distances;
};

cbuffer CascadeBuffer : register(b2)
{
	CascadeData CascData;
};

[instance(CASCADE_COUNT + 1)]
[maxvertexcount(3)]
void GSMain(triangle GS_IN p[3], in uint id : SV_GSInstanceID, inout TriangleStream<GS_OUT> stream)
{
	[unroll]
	for (int i = 0; i < 3; ++i)
	{
		GS_OUT gs = (GS_OUT)0;
		gs.pos = mul(float4(p[i].pos.xyz, 1.0f), CascData.ViewProj[id]);
		gs.arrInd = id;
		stream.Append(gs);
	}
}
