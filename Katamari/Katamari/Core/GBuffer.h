#pragma once

#include "MinimalCore.h"

class GBuffer
{
public:
    GBuffer();

    void Initialize();

public:
    ID3D11Texture2D* diffuseBuffer;
    ID3D11ShaderResourceView* diffuseSRV;
    ID3D11RenderTargetView* diffuseRTV;

    ID3D11Texture2D* normalBuffer;
    ID3D11ShaderResourceView* normalSRV;
    ID3D11RenderTargetView* normalRTV;

    ID3D11Texture2D* positionBuffer;
    ID3D11ShaderResourceView* positionSRV;
    ID3D11RenderTargetView* positionRTV;

private:
    Game* game;
};
