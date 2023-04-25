#pragma once

#include "../MinimalCore.h"

class BitonicSort
{
public:
    BitonicSort();

    void Initialize(unsigned int maxParticlesCount);
    void Sort();

    ID3D11ShaderResourceView* SortSRV;
    ID3D11UnorderedAccessView* SortUAV;

private:

#pragma pack(push, 4)
    struct ParticleDepths
    {
        UINT Index;
        float Depth;
    };
#pragma pack(pop)

    struct CBSort
    {
        UINT iLevel;
        UINT iLevelMask;
        UINT iWidth;
        UINT iHeight;
    };

    Game* game = nullptr;
    unsigned int _maxParticlesCount;

    ID3D11Buffer* constSortBuffer;
    ID3D11Buffer* sortBuffer;

    ID3D11ComputeShader* sortShader;
    ID3D11ComputeShader* transposeShader;

    void SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight);
};

