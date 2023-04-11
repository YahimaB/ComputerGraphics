#include "LightComponent.h"

LightComponent* LightComponent::Instance = 0;


LightComponent::LightComponent()
{
	Game = ::Game::Instance;
	Camera = ::Camera::Current;
	Instance = this;
}

void LightComponent::Initialize()
{
    for (int i = 0; i < CascadeCount; i++)
    {
        shadowCascadeLevels[i] = (Camera->ClipPlanes.Far / cascadeOffsets[i]);
    }

	D3D11_BUFFER_DESC constBufPerSceneDesc = {};
	constBufPerSceneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufPerSceneDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufPerSceneDesc.CPUAccessFlags = 0;
	constBufPerSceneDesc.MiscFlags = 0;
	constBufPerSceneDesc.StructureByteStride = 0;
	constBufPerSceneDesc.ByteWidth = sizeof(LightingData);

	Game->device->CreateBuffer(&constBufPerSceneDesc, nullptr, &constLightsBuffer);

    D3D11_BUFFER_DESC constCascadeBufDesc = {};
    constCascadeBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constCascadeBufDesc.Usage = D3D11_USAGE_DEFAULT;
    constCascadeBufDesc.CPUAccessFlags = 0;
    constCascadeBufDesc.MiscFlags = 0;
    constCascadeBufDesc.StructureByteStride = 0;
    constCascadeBufDesc.ByteWidth = sizeof(CascadeData);

    Game->device->CreateBuffer(&constCascadeBufDesc, nullptr, &constCascadeBuffer);

    D3D11_SAMPLER_DESC depthSamplerStateDesc = {};
    depthSamplerStateDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    depthSamplerStateDesc.ComparisonFunc = D3D11_COMPARISON_GREATER_EQUAL;
    depthSamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    depthSamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    depthSamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    depthSamplerStateDesc.BorderColor[0] = 1.0f;
    depthSamplerStateDesc.BorderColor[1] = 1.0f;
    depthSamplerStateDesc.BorderColor[2] = 1.0f;
    depthSamplerStateDesc.BorderColor[3] = 1.0f;

    Game->device->CreateSamplerState(&depthSamplerStateDesc, &depthSamplerState);
}

void LightComponent::Update(float deltaTime)
{
	Vector3 viewVector = Camera->Transform->GetModel().Backward();

    Vector3 scale, pos;
    Quaternion rot;
    Transform->GetModel().Decompose(scale, rot, pos);

	LightingData lightProps = {};
    lightProps.Lights[0].Position = Vector4(pos);
    lightProps.Lights[0].Direction = Vector4(-1.0f, -1.0f, 0.0f, 0.0f);
    lightProps.Lights[0].Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f;
    lightProps.Lights[0].Enabled = true;

    lightProps.ViewMatrix = Camera->GetViewMatrix();
	lightProps.ViewVector = viewVector;
	lightProps.Intensity = 5.0f;

	Game->context->UpdateSubresource(constLightsBuffer, 0, nullptr, &lightProps, 0, 0);

    CascadeData cascadeData = {};
    auto projViewMatrices = GetLightProjViewMatrices();
    for (int i = 0; i < 5; ++i)
    {
        cascadeData.ViewProj[i] = projViewMatrices[i];
    }
    cascadeData.Distance = Vector4(shadowCascadeLevels);

    Game->context->UpdateSubresource(constCascadeBuffer, 0, nullptr, &cascadeData, 0, 0);
}

void LightComponent::PrepareFrame()
{
    Game->context->GSSetConstantBuffers(2, 1, &constCascadeBuffer);
}

void LightComponent::Draw()
{
	Game->context->PSSetConstantBuffers(1, 1, &constLightsBuffer);
    Game->context->PSSetConstantBuffers(2, 1, &constCascadeBuffer);

    Game->context->PSSetSamplers(1, 1, &depthSamplerState);
}

void LightComponent::DestroyResources()
{
	constLightsBuffer->Release();

	Game = nullptr;

	GameComponent::DestroyResources();
}

Vector4 LightComponent::GetShadowCascadeDistances() const
{
    return Vector4(shadowCascadeLevels[0], shadowCascadeLevels[1], shadowCascadeLevels[2], shadowCascadeLevels[3]);
}

Matrix LightComponent::GetLightProjViewMatrix(const float nearPlane, const float farPlane)
{
    const auto proj = Matrix::CreatePerspectiveFieldOfView(
        Camera::Current->FOV, Camera::Current->AspectRatio,
        nearPlane, farPlane
    );
    const auto corners = Camera::Current->GetFrustumCornersWorldSpace(Camera::Current->GetViewMatrix(), proj);

    auto center = Vector3::Zero;
    for (const auto& v : corners)
    {
        center += Vector3(v);
    }
    center /= static_cast<float>(corners.size());

    const auto lightView = Matrix::CreateLookAt(center - Vector4(-1.0f, -1.0f, 0.0f, 0.0f), center, Vector3::Up);

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::lowest();
    for (const auto& v : corners)
    {
        const auto trf = Vector4::Transform(v, lightView);
        minX = (std::min)(minX, trf.x);
        maxX = (std::max)(maxX, trf.x);
        minY = (std::min)(minY, trf.y);
        maxY = (std::max)(maxY, trf.y);
        minZ = (std::min)(minZ, trf.z);
        maxZ = (std::max)(maxZ, trf.z);
    }

    constexpr float zMult = 10.0f; // how much geometry to include from outside the view frustum
    minZ = (minZ < 0) ? minZ * zMult : minZ / zMult;
    maxZ = (maxZ < 0) ? maxZ / zMult : maxZ * zMult;

    const auto lightProjection = Matrix::CreateOrthographicOffCenter(minX, maxX, minY, maxY, minZ, maxZ);

    return lightView * lightProjection;
}

std::vector<Matrix> LightComponent::GetLightProjViewMatrices()
{
    std::vector<Matrix> ret;
    for (size_t i = 0; i < CascadeCount + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(GetLightProjViewMatrix(Camera::Current->ClipPlanes.Near, shadowCascadeLevels[i]));
        }
        else if (i < CascadeCount)
        {
            ret.push_back(GetLightProjViewMatrix(shadowCascadeLevels[i - 1], shadowCascadeLevels[i]));
        }
        else
        {
            ret.push_back(GetLightProjViewMatrix(shadowCascadeLevels[i - 1], Camera::Current->ClipPlanes.Far));
        }
    }
    return ret;
}
