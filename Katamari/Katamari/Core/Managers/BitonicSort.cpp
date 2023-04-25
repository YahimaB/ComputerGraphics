#include "BitonicSort.h"

#include "../Game.h"

const UINT BITONIC_BLOCK_SIZE = 512;
const UINT TRANSPOSE_BLOCK_SIZE = 16;
const UINT MATRIX_WIDTH = BITONIC_BLOCK_SIZE;


BitonicSort::BitonicSort()
{
    game = ::Game::Instance;
}

void BitonicSort::Initialize(unsigned int maxParticlesCount)
{
    _maxParticlesCount = maxParticlesCount;

    ID3DBlob* comSortRes;
    ID3DBlob* comTransposeRes;
    ID3DBlob* errorCode;

    D3DCompileFromFile(L"./Shaders/SortShader.hlsl", nullptr, nullptr, "BitonicSort", "cs_5_0",
        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &comSortRes, &errorCode);
    D3DCompileFromFile(L"./Shaders/SortShader.hlsl", nullptr, nullptr, "BitonicSort", "cs_5_0",
        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &comTransposeRes, &errorCode);

    game->device->CreateComputeShader(comSortRes->GetBufferPointer(), comSortRes->GetBufferSize(), nullptr, &sortShader);
    game->device->CreateComputeShader(comTransposeRes->GetBufferPointer(), comTransposeRes->GetBufferSize(), nullptr, &transposeShader);


    D3D11_BUFFER_DESC constSortBufDesc = {};
    constSortBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constSortBufDesc.Usage = D3D11_USAGE_DEFAULT;
    constSortBufDesc.MiscFlags = 0;
    constSortBufDesc.CPUAccessFlags = 0;
    constSortBufDesc.ByteWidth = sizeof(CBSort);

    game->device->CreateBuffer(&constSortBufDesc, nullptr, &constSortBuffer);

    D3D11_BUFFER_DESC particleDepthBufDesc = {};
    particleDepthBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    particleDepthBufDesc.Usage = D3D11_USAGE_DEFAULT;
    particleDepthBufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    particleDepthBufDesc.CPUAccessFlags = 0;
    particleDepthBufDesc.StructureByteStride = sizeof(ParticleDepths);
    particleDepthBufDesc.ByteWidth = _maxParticlesCount * sizeof(ParticleDepths);

    ParticleDepths* depthsData = new ParticleDepths[_maxParticlesCount];
    for (UINT i = 0; i < _maxParticlesCount; ++i)
    {
        depthsData[i] = { i, 0.0f };
    }

    D3D11_SUBRESOURCE_DATA initSortData = {};
    initSortData.pSysMem = depthsData;
    initSortData.SysMemPitch = 0;
    initSortData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&particleDepthBufDesc, &initSortData, &sortBuffer);

    game->device->CreateShaderResourceView(sortBuffer, nullptr, &SortSRV);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDepthDesc = {};
    uavDepthDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDepthDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDepthDesc.Buffer = D3D11_BUFFER_UAV{
        0,
        _maxParticlesCount,
        0
    };

    game->device->CreateUnorderedAccessView(sortBuffer, &uavDepthDesc, &SortUAV);
}

//--------------------------------------------------------------------------------------
// Helper to set the compute shader constants
//--------------------------------------------------------------------------------------
void BitonicSort::SetConstants(UINT iLevel, UINT iLevelMask, UINT iWidth, UINT iHeight)
{
    CBSort cb = { iLevel, iLevelMask, iWidth, iHeight };
    game->context->UpdateSubresource(constSortBuffer, 0, nullptr, &cb, 0, 0);
    game->context->CSSetConstantBuffers(0, 1, &constSortBuffer);
}
//--------------------------------------------------------------------------------------
// GPU Bitonic Sort
//--------------------------------------------------------------------------------------
void BitonicSort::Sort()
{
    const UINT MATRIX_HEIGHT = _maxParticlesCount / BITONIC_BLOCK_SIZE;

    // Sort the data
    // First sort the rows for the levels <= to the block size
    for (UINT level = 2; level <= BITONIC_BLOCK_SIZE; level = level * 2)
    {
        SetConstants(level, level, MATRIX_HEIGHT, MATRIX_WIDTH);

        // Sort the row data
        game->context->CSSetUnorderedAccessViews(0, 1, &SortUAV, nullptr);
        game->context->CSSetShader(sortShader, nullptr, 0);
        game->context->Dispatch(_maxParticlesCount / BITONIC_BLOCK_SIZE, 1, 1);
    }

    // Then sort the rows and columns for the levels > than the block size
    // Transpose. Sort the Columns. Transpose. Sort the Rows.
    for (UINT level = (BITONIC_BLOCK_SIZE * 2); level <= _maxParticlesCount; level = level * 2)
    {
        SetConstants((level / BITONIC_BLOCK_SIZE), (level & ~_maxParticlesCount) / BITONIC_BLOCK_SIZE, MATRIX_WIDTH, MATRIX_HEIGHT);

        // Transpose the data from buffer 1 into buffer 2
        ID3D11ShaderResourceView* pViewnullptr = nullptr;
        game->context->CSSetShaderResources(0, 1, &pViewnullptr);
        game->context->CSSetUnorderedAccessViews(0, 1, &SortUAV, nullptr);
        game->context->CSSetShader(transposeShader, nullptr, 0);
        game->context->Dispatch(MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, 1);

        // Sort the transposed column data
        game->context->CSSetShader(sortShader, nullptr, 0);
        game->context->Dispatch(_maxParticlesCount / BITONIC_BLOCK_SIZE, 1, 1);

        SetConstants(BITONIC_BLOCK_SIZE, level, MATRIX_HEIGHT, MATRIX_WIDTH);

        // Transpose the data from buffer 2 back into buffer 1
        game->context->CSSetShaderResources(0, 1, &pViewnullptr);
        game->context->CSSetUnorderedAccessViews(0, 1, &SortUAV, nullptr);
        game->context->CSSetShader(transposeShader, nullptr, 0);
        game->context->Dispatch(MATRIX_HEIGHT / TRANSPOSE_BLOCK_SIZE, MATRIX_WIDTH / TRANSPOSE_BLOCK_SIZE, 1);

        // Sort the row data
        game->context->CSSetShader(sortShader, nullptr, 0);
        game->context->Dispatch(_maxParticlesCount / BITONIC_BLOCK_SIZE, 1, 1);
    }
}