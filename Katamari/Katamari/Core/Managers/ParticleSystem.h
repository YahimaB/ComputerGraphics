#pragma once
#include <map>

#include "../MinimalCore.h"
#include "../GameComponent.h"
#include "BitonicSort.h"

#include "../magic_enum.hpp"

using namespace magic_enum::bitwise_operators;

class ParticleSystem : public GameComponent
{
private:
    ID3D11Buffer* bufFirst;
    ID3D11Buffer* bufSecond;
    ID3D11Buffer* countBuf;
    ID3D11Buffer* injectionBuf;
    ID3D11Buffer* constBuf;

    ID3D11ShaderResourceView* srvFirst;
    ID3D11ShaderResourceView* srvSecond;
    ID3D11ShaderResourceView* srvSrc;
    ID3D11ShaderResourceView* srvDst;

    ID3D11UnorderedAccessView* uavFirst;
    ID3D11UnorderedAccessView* uavSecond;
    ID3D11UnorderedAccessView* uavSrc;
    ID3D11UnorderedAccessView* uavDst;
    ID3D11UnorderedAccessView* injUav;

    template<typename T>
    std::vector<D3D_SHADER_MACRO> GetMacros(T flags);

public:
    float Width;
    float Height;
    float Length;

    const unsigned int MaxParticlesCount = 256 * 256;
    const unsigned int MaxParticlesInjectionCount = 1000;

    unsigned int InjectionCount = 0;
    unsigned int ParticlesCount = MaxParticlesCount;

#pragma pack(push, 4)
    struct Particle
    {
        Vector4 Position;
        Vector4 Velocity;
        Vector4 Color0;
        Vector2 Size0Size1;
        float LifeTime;
    };
#pragma pack(pop)

#pragma pack(push, 4)
    struct ConstData
    {
        Matrix World;
        Matrix View;
        Matrix Proj;
        Vector4 DeltaTimeMaxParticlesGroupdim;
    };
#pragma pack(pop)

    struct DrawArgs
    {
        UINT IndexCountPerInstance;
        UINT InstanceCount;
        UINT StartIndexLocation;
        int BaseVertexLocation;
        UINT StartInstanceLocation;
    };

    enum class ComputeFlags
    {
        INJECTION = 1 << 0,
        SIMULATION = 1 << 1,
        ADD_GRAVITY = 1 << 2,
    };

    std::map<ComputeFlags, ID3D11ComputeShader*> ComputeShaders;

    ConstData constData;

    Particle* injectionParticles = new Particle[MaxParticlesInjectionCount];

    ID3D11Buffer* bufArgs;
    ID3D11Buffer* indexBuffer;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;

    ID3D11RasterizerState* rastState;
    ID3D11BlendState* blendState;
    ID3D11DepthStencilState* depthState;

    Game* game = nullptr;
    BitonicSort* bitonicSort = new BitonicSort();

    ParticleSystem();

    static void GetGroupSize(int particleCount, int& groupSizeX, int& groupSizeY);

    void Initialize() override;
    void Update(float deltaTime) override;
    void Draw() override;
    void DestroyResources() override;

    void Emit(float deltaTime);
    void LoadShaders();
    void CreateBuffers();
    void AddParticle(const Particle& p);
    void SwapBuffers();

public:
        
    struct ParticleSettings
    {
        float SpawnInterval;
        Vector3 Velocity;
        Vector4 Color0;
        Vector2 Size0Size1;
        float LifeTime;
    };

    ParticleSettings ParticleSettings = {0.01f, Vector3::Zero, Vector4::One, Vector2::One, 1.0f};
    float accumulatedTime = 0;
};
