#include "GBuffer.h"

#include "Game.h"

GBuffer::GBuffer()
{
    game = Game::Instance;
}

void GBuffer::Initialize()
{
    const int width = game->Display->ClientWidth;
    const int height = game->Display->ClientHeight;

    D3D11_TEXTURE2D_DESC baseBuffDesc = {};
    baseBuffDesc.Width = width;
    baseBuffDesc.Height = height;
    baseBuffDesc.ArraySize = 1;
    baseBuffDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    baseBuffDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseBuffDesc.MipLevels = 1;
    baseBuffDesc.SampleDesc.Count = 1;
    baseBuffDesc.SampleDesc.Quality = 0;
    baseBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    baseBuffDesc.CPUAccessFlags = 0;
    baseBuffDesc.MiscFlags = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC baseSrvDesc = {};
    baseSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    baseSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseSrvDesc.Texture2D.MipLevels = 1;
    baseSrvDesc.Texture2D.MostDetailedMip = 0;

    D3D11_RENDER_TARGET_VIEW_DESC baseRtvDesc = {};
    baseRtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    baseRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    baseRtvDesc.Texture2D.MipSlice = 0;

    game->device->CreateTexture2D(&baseBuffDesc, nullptr, &diffuseBuffer);
    game->device->CreateShaderResourceView(diffuseBuffer, &baseSrvDesc, &diffuseSRV);
    game->device->CreateRenderTargetView(diffuseBuffer, &baseRtvDesc, &diffuseRTV);

    baseBuffDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    baseSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    baseRtvDesc.Format = DXGI_FORMAT_R8G8B8A8_SNORM;
    game->device->CreateTexture2D(&baseBuffDesc, nullptr, &normalBuffer);
    game->device->CreateShaderResourceView(normalBuffer, &baseSrvDesc, &normalSRV);
    game->device->CreateRenderTargetView(normalBuffer, &baseRtvDesc, &normalRTV);

    baseBuffDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    baseSrvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    baseRtvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    game->device->CreateTexture2D(&baseBuffDesc, nullptr, &positionBuffer);
    game->device->CreateShaderResourceView(positionBuffer, &baseSrvDesc, &positionSRV);
    game->device->CreateRenderTargetView(positionBuffer, &baseRtvDesc, &positionRTV);
}
