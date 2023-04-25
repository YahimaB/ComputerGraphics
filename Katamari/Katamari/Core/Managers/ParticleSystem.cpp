#include "ParticleSystem.h"

#include "../Components/RendererComponent.h"
#include "../Game.h"
#include "ShaderManager.h"


template<typename T>
std::vector<D3D_SHADER_MACRO> ParticleSystem::GetMacros(T flags)
{
    std::vector<D3D_SHADER_MACRO> macros;

    constexpr auto& entries = magic_enum::enum_entries<T>();

    for (const std::pair<T, std::string_view>& p : entries)
    {
        if (static_cast<uint32_t>(flags & p.first) > 0) {
            D3D_SHADER_MACRO macro;
            macro.Name = p.second.data();
            macro.Definition = "1";

            macros.push_back(macro);
        }
    }

    macros.push_back(D3D_SHADER_MACRO{ nullptr, nullptr });
    return macros;
}

static float RandomFloatInRange(float low, float high)
{
    return (high - low) * rand() / RAND_MAX + low;
}

static Vector4 RandomVectorInRange(Vector3 range)
{
    Vector4 res = Vector4::Zero;
    res.x = RandomFloatInRange(-range.x, range.x);
    res.y = RandomFloatInRange(-range.y, range.y);
    res.z = RandomFloatInRange(-range.z, range.z);
    return res;
}

ParticleSystem::ParticleSystem()
{
    game = ::Game::Instance;

    Width = 1.0f;
    Height = 1.0f;
    Length = 1.0f;
}

void ParticleSystem::Initialize()
{
    LoadShaders();
    CreateBuffers();
    bitonicSort->Initialize(MaxParticlesCount);

    D3D11_RASTERIZER_DESC rastDesc = {};
    rastDesc.CullMode = D3D11_CULL_BACK;
    rastDesc.FrontCounterClockwise = true;
    rastDesc.FillMode = D3D11_FILL_SOLID;

    game->device->CreateRasterizerState(&rastDesc, &rastState);


    D3D11_BLEND_DESC blendDesc = D3D11_BLEND_DESC{ false, false };
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    game->device->CreateBlendState(&blendDesc, &blendState);


    D3D11_DEPTH_STENCIL_DESC depthDesc = {};
    depthDesc.DepthEnable = true;
    depthDesc.StencilEnable = false;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthDesc.StencilReadMask = 0x00;
    depthDesc.StencilWriteMask = 0x00;

    game->device->CreateDepthStencilState(&depthDesc, &depthState);
}

void ParticleSystem::Emit(float deltaTime)
{
    accumulatedTime += deltaTime;

    int spawned = 0;
    while (accumulatedTime > ParticleSettings.SpawnInterval)
    {
        //srand(std::time(nullptr) + spawned);

        Particle p = {};
        p.Position.x = RandomFloatInRange(-Width, Width);
        p.Position.y = RandomFloatInRange(-Height, Height);
        p.Position.z = RandomFloatInRange(-Length, Length);
        p.Position.w = 1.0f;

        p.Velocity = Vector4(ParticleSettings.Velocity);
        p.Color0 = ParticleSettings.Color0;
        p.Size0Size1 = ParticleSettings.Size0Size1;

        p.LifeTime = ParticleSettings.LifeTime;
        AddParticle(p);
        accumulatedTime -= ParticleSettings.SpawnInterval;
        spawned++;
    }
}

void ParticleSystem::Update(float deltaTime)
{
    deltaTime /= 1000;
    //std::cout << deltaTime << std::endl;
    Emit(deltaTime);
    

    int groupSizeX, groupSizeY;
    GetGroupSize(ParticlesCount, groupSizeX, groupSizeY);

    constData.World = Transform->GetModel();
    constData.View = Camera::Current->GetViewMatrix();
    constData.Proj = Camera::Current->GetProjectionMatrix();
    constData.DeltaTimeMaxParticlesGroupdim = Vector4(deltaTime, ParticlesCount, groupSizeY, 0);

    game->context->UpdateSubresource(constBuf, 0, nullptr, &constData, 0, 0);
    game->context->CSSetConstantBuffers(0, 1, &constBuf);

    constexpr UINT counterKeepValue = -1;
    constexpr UINT counterZero = 0;

    game->context->CSSetUnorderedAccessViews(0, 1, &uavSrc, &counterKeepValue);
    game->context->CSSetUnorderedAccessViews(1, 1, &uavDst, &counterZero);

    game->context->CSSetShaderResources(1, 1, &game->depthSRV);
    game->context->CSSetShaderResources(2, 1, &game->gBuffer->normalSRV);

    D3D11_SAMPLER_DESC samplerStateDesc = {};
    samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDesc.MinLOD = 0.0f;
    samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* samplerState;
    game->device->CreateSamplerState(&samplerStateDesc, &samplerState);
    game->context->CSSetSamplers(0, 1, &samplerState);

    game->context->CSSetShader(ComputeShaders[ComputeFlags::SIMULATION | ComputeFlags::ADD_GRAVITY], nullptr, 0);

    if (groupSizeX > 0)
    {
        game->context->Dispatch(groupSizeX, groupSizeY, 1);
    }

    if (InjectionCount > 0)
    {
        int injSizeX, injSizeY;
        GetGroupSize(InjectionCount, injSizeX, injSizeY);

        constData.DeltaTimeMaxParticlesGroupdim = Vector4(deltaTime, InjectionCount, injSizeY, 0);

        game->context->UpdateSubresource(constBuf, 0, nullptr, &constData, 0, 0);
        game->context->CSSetConstantBuffers(0, 1, &constBuf);

        game->context->UpdateSubresource(injectionBuf, 0, nullptr, injectionParticles, 0, 0);

        game->context->CSSetUnorderedAccessViews(0, 1, &injUav, &InjectionCount);
        game->context->CSSetShader(ComputeShaders[ComputeFlags::INJECTION], nullptr, 0);

        game->context->Dispatch(injSizeX, injSizeY, 1);

        InjectionCount = 0;
    }

    ID3D11UnorderedAccessView* nuPtr = nullptr;
    game->context->CSSetUnorderedAccessViews(0, 1, &nuPtr, &counterZero);
    game->context->CSSetUnorderedAccessViews(1, 1, &nuPtr, &counterZero);

    game->context->CopyStructureCount(countBuf, 0, uavDst);

    D3D11_MAPPED_SUBRESOURCE subresource;
    game->context->Map(countBuf, 0, D3D11_MAP_READ, 0, &subresource);

    UINT* data = reinterpret_cast<UINT*>(subresource.pData);
    ParticlesCount = data[0];

    game->context->Unmap(countBuf, 0);

    SwapBuffers();

    //bitonicSort->Sort();
}

void ParticleSystem::Draw()
{
    game->context->ClearState();
    game->context->OMSetRenderTargets(1, &game->mainRTV, game->mainDSV);
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(game->Display->ClientWidth);
    viewport.Height = static_cast<float>(game->Display->ClientHeight);
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;
    game->context->RSSetViewports(1, &viewport);

    ID3D11RasterizerState* oldState = nullptr;
    game->context->RSGetState(&oldState);
    game->context->RSSetState(rastState);

    ID3D11BlendState* oldBlend = nullptr;
    UINT oldMask = 0;
    float oldBlendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    game->context->OMGetBlendState(&oldBlend, oldBlendFactor, &oldMask);

    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    game->context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    ID3D11DepthStencilState* oldDepthState = nullptr;
    UINT oldStencilRef = 0;
    game->context->OMGetDepthStencilState(&oldDepthState, &oldStencilRef);
    game->context->OMSetDepthStencilState(depthState, 0);

    game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    game->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    game->context->VSSetShader(vertexShader, nullptr, 0);
    game->context->PSSetShader(pixelShader, nullptr, 0);

    game->context->VSSetConstantBuffers(0, 1, &constBuf);
    game->context->VSSetShaderResources(0, 1, &srvSrc);
    game->context->VSSetShaderResources(1, 1, &bitonicSort->SortSRV);
    
    game->context->DrawIndexedInstancedIndirect(bufArgs, 0);


    game->context->OMSetBlendState(oldBlend, oldBlendFactor, oldMask);
    game->context->RSSetState(oldState);
    game->context->OMSetDepthStencilState(oldDepthState, oldStencilRef);
}

void ParticleSystem::DestroyResources()
{
}

void ParticleSystem::LoadShaders()
{
    ID3DBlob* verRes;
    ID3DBlob* pixRes;
    ID3DBlob* errorCode;

    D3DCompileFromFile(L"./Shaders/Particles.hlsl", nullptr, nullptr, "VSMain", "vs_5_0",
        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &verRes, &errorCode);
    D3DCompileFromFile(L"./Shaders/Particles.hlsl", nullptr, nullptr, "PSMain", "ps_5_0",
        D3DCOMPILE_PACK_MATRIX_ROW_MAJOR, 0, &pixRes, &errorCode);

    game->device->CreateVertexShader(verRes->GetBufferPointer(), verRes->GetBufferSize(), nullptr, &vertexShader);
    game->device->CreatePixelShader(pixRes->GetBufferPointer(), pixRes->GetBufferSize(), nullptr, &pixelShader);


    std::vector<ComputeFlags> flags = {
        ComputeFlags::INJECTION,
        ComputeFlags::SIMULATION,
        ComputeFlags::SIMULATION | ComputeFlags::ADD_GRAVITY,
    };

    for (auto& flag : flags)
    {
        auto macros = GetMacros(flag);
        ID3DBlob* comRes;
        ID3DBlob* error;
        D3DCompileFromFile(L"./Shaders/ParticlesCompute.hlsl",
            &macros[0],
            nullptr,
            "CSMain",
            "cs_5_0",
            D3DCOMPILE_PACK_MATRIX_ROW_MAJOR,
            0,
            &comRes,
            &error);

        ID3D11ComputeShader* compShader;
        game->device->CreateComputeShader(comRes->GetBufferPointer(), comRes->GetBufferSize(), nullptr, &compShader);

        ComputeShaders.emplace(flag, compShader);
    }
}

void ParticleSystem::CreateBuffers()
{
    D3D11_BUFFER_DESC constBufDesc = {};
    constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constBufDesc.Usage = D3D11_USAGE_DEFAULT;
    constBufDesc.MiscFlags = 0;
    constBufDesc.CPUAccessFlags = 0;
    constBufDesc.ByteWidth = sizeof(ConstData);

    game->device->CreateBuffer(&constBufDesc, nullptr, &constBuf);

    D3D11_BUFFER_DESC indexBufDesc;
    indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufDesc.CPUAccessFlags = 0;
    indexBufDesc.MiscFlags = 0;
    indexBufDesc.StructureByteStride = 0;
    indexBufDesc.ByteWidth = sizeof(UINT) * MaxParticlesCount * 6;

    D3D11_SUBRESOURCE_DATA indexData;
    UINT* indices = new UINT[MaxParticlesCount * 6];
    for (UINT i = 0; i < MaxParticlesCount; ++i)
    {
        indices[i * 6 + 0] = i * 4 + 0;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;

        indices[i * 6 + 3] = i * 4 + 1;
        indices[i * 6 + 4] = i * 4 + 0;
        indices[i * 6 + 5] = i * 4 + 3;
    }
    indexData.pSysMem = &indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    game->device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

    delete[] indices;

    D3D11_SUBRESOURCE_DATA argsData;
    DrawArgs args = {};
    args.IndexCountPerInstance = MaxParticlesCount * 6;
    args.InstanceCount = 1;
    argsData.pSysMem = &args;
    argsData.SysMemPitch = 0;
    argsData.SysMemSlicePitch = 0;

    D3D11_BUFFER_DESC argsBufDesc = {};
    argsBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    argsBufDesc.Usage = D3D11_USAGE_DEFAULT;
    argsBufDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
    argsBufDesc.CPUAccessFlags = 0;
    argsBufDesc.StructureByteStride = sizeof(DrawArgs);
    argsBufDesc.ByteWidth = sizeof(DrawArgs);

    auto res = game->device->CreateBuffer(&argsBufDesc, &argsData, &bufArgs);


    D3D11_BUFFER_DESC particleBufDesc = {};
    particleBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    particleBufDesc.Usage = D3D11_USAGE_DEFAULT;
    particleBufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    particleBufDesc.CPUAccessFlags = 0;
    particleBufDesc.StructureByteStride = sizeof(Particle);
    particleBufDesc.ByteWidth = MaxParticlesCount * sizeof(Particle);

    game->device->CreateBuffer(&particleBufDesc, nullptr, &bufFirst);
    game->device->CreateBuffer(&particleBufDesc, nullptr, &bufSecond);


    game->device->CreateShaderResourceView(bufFirst, nullptr, &srvFirst);
    game->device->CreateShaderResourceView(bufSecond, nullptr, &srvSecond);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer = D3D11_BUFFER_UAV{
        0,
        MaxParticlesCount,
        D3D11_BUFFER_UAV_FLAG_APPEND
    };

    game->device->CreateUnorderedAccessView(bufFirst, &uavDesc, &uavFirst);
    game->device->CreateUnorderedAccessView(bufSecond, &uavDesc, &uavSecond);

    srvSrc = srvFirst;
    uavSrc = uavFirst;
    srvDst = srvSecond;
    uavDst = uavSecond;

    ID3D11UnorderedAccessView* nuPtr = nullptr;
    game->context->CSSetUnorderedAccessViews(0, 1, &uavSrc, &MaxParticlesCount);
    game->context->CSSetUnorderedAccessViews(0, 1, &nuPtr, nullptr);


    D3D11_BUFFER_DESC countBufDesc = {};
    countBufDesc.BindFlags = 0;
    countBufDesc.Usage = D3D11_USAGE_STAGING;
    countBufDesc.MiscFlags = 0;
    countBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    countBufDesc.StructureByteStride = 0;
    countBufDesc.ByteWidth = 4;

    game->device->CreateBuffer(&countBufDesc, nullptr, &countBuf);


    D3D11_BUFFER_DESC injBufDesc = {};
    injBufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    injBufDesc.Usage = D3D11_USAGE_DEFAULT;
    injBufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    injBufDesc.CPUAccessFlags = 0;
    injBufDesc.StructureByteStride = sizeof(Particle);
    injBufDesc.ByteWidth = MaxParticlesInjectionCount * sizeof(Particle);

    game->device->CreateBuffer(&injBufDesc, nullptr, &injectionBuf);


    D3D11_UNORDERED_ACCESS_VIEW_DESC injUavDesc = {};
    injUavDesc.Format = DXGI_FORMAT_UNKNOWN;
    injUavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    injUavDesc.Buffer = D3D11_BUFFER_UAV{
        0,
        MaxParticlesInjectionCount,
        D3D11_BUFFER_UAV_FLAG_APPEND
    };

    game->device->CreateUnorderedAccessView(injectionBuf, &injUavDesc, &injUav);    
}

void ParticleSystem::AddParticle(const Particle& p)
{
    injectionParticles[InjectionCount++] = p;
}

void ParticleSystem::SwapBuffers()
{
    auto tmpSrv = srvSrc;
    auto tmpUav = uavSrc;

    srvSrc = srvDst;
    uavSrc = uavDst;

    srvDst = tmpSrv;
    uavDst = tmpUav;
}

void ParticleSystem::GetGroupSize(int particleCount, int& groupSizeX, int& groupSizeY)
{
    int numGroups = (particleCount % 256 != 0) ? ((particleCount / 256) + 1) : (particleCount / 256);
    auto root = std::sqrt(static_cast<double>(numGroups));
    root = std::ceil(root);
    groupSizeX = static_cast<int>(root);
    groupSizeY = groupSizeX;
}

