#pragma once

#include "MinimalCore.h"

class GBuffer
{
public:
    GBuffer();

    void Initialize();

public:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> albedoBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> albedoSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> albedoRtv_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> normalBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> normalRtv_;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> positionBuffer_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> positionSrv_;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> positionRtv_;

private:
    Game* game_;
};
