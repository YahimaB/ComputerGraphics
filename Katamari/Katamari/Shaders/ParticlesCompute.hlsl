#pragma pack_matrix(row_major)

float nrand(float2 uv)
{
    return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

struct Particle
{
    float4 Position;
    float4 Velocity;
    float4 Color0;
    float2 Size0Size1;
    float LifeTime;
};

struct ParticleDepths
{
    float Index;
    float Depth;
};

cbuffer ConstBuff : register(b0)
{
    float4x4 gWorld;
    float4x4 gView;
    float4x4 gProj;
    float4 gDeltaTimeMaxParticlesGroupdim;
};

ConsumeStructuredBuffer<Particle> particlesBufSrc : register(u0);
AppendStructuredBuffer<Particle> particlesBufDst : register(u1);
RWStructuredBuffer<ParticleDepths> sortedBufSrc : register(u2);

#define BLOCK_SIZE 256
#define THREAD_IN_GROUP_TOTAL 256

[numthreads(BLOCK_SIZE, 1, 1)]
void CSMain(
        uint3 groupID : SV_GroupID,
        uint3 groupThreadID : SV_GroupThreadID,
        uint3 dispatchThreadID : SV_DispatchThreadID,
        uint groupIndex : SV_GroupIndex
    )
{
    uint id = groupID.x * THREAD_IN_GROUP_TOTAL + groupID.y * gDeltaTimeMaxParticlesGroupdim.z * THREAD_IN_GROUP_TOTAL + groupIndex;

    [flatten]
    if (id >= (uint)gDeltaTimeMaxParticlesGroupdim.y)
        return;
    
#ifdef INJECTION
    Particle p = particlesBufSrc.Consume();
    if (p.LifeTime > 0)
    {
        particlesBufDst.Append(p);
    }
#endif
    
#ifdef SIMULATION
    Particle p = particlesBufSrc.Consume();
    p.LifeTime -= gDeltaTimeMaxParticlesGroupdim.x;

    if (p.LifeTime > 0)
    {
#ifdef ADD_GRAVITY
        p.Velocity += float4(0.0f, -9.8f * gDeltaTimeMaxParticlesGroupdim.x, 0.0f, 0.0f);
#endif
        p.Position.xyz += p.Velocity * gDeltaTimeMaxParticlesGroupdim.x;

        if (p.Position.y <= -(gWorld[3].y - 0.1f))
        {
            p.Velocity.y /= -4;
            p.Velocity.x = 2 * p.Velocity.y * (nrand(p.Position.xy) - 0.5f);
            p.Velocity.z = 2 * p.Velocity.y * (nrand(p.Position.xy) - 0.5f);
        }


        particlesBufDst.Append(p);

        sortedBufSrc[id].Index = id;
        float4 tmpDepth = mul(float4(p.Position.xyz, 1.0f), gWorld);
        tmpDepth = mul(tmpDepth, gView);
        sortedBufSrc[id].Depth = tmpDepth.z;
    }
#endif
}
