#include "LightComponent.h"

LightComponent* LightComponent::Instance = 0;


LightComponent::LightComponent()
{
	Game = ::Game::Instance;
	Camera = ::Camera::Current;
	Instance = this;

    shadowCascadeLevels_.push_back(1000.0f / 50.0f);
    shadowCascadeLevels_.push_back(1000.0f / 25.0f);
    shadowCascadeLevels_.push_back(1000.0f / 10.0f);
    shadowCascadeLevels_.push_back(1000.0f / 2.0f);
}

void LightComponent::Initialize()
{
	D3D11_BUFFER_DESC constBufPerSceneDesc = {};
	constBufPerSceneDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufPerSceneDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufPerSceneDesc.CPUAccessFlags = 0;
	constBufPerSceneDesc.MiscFlags = 0;
	constBufPerSceneDesc.StructureByteStride = 0;
	constBufPerSceneDesc.ByteWidth = sizeof(LightProperties);

	Game->device->CreateBuffer(&constBufPerSceneDesc, nullptr, &constBuffer);
}

void LightComponent::Update(float deltaTime)
{
	Vector3 viewVector = Camera->Transform->GetModel().Backward();

	LightProperties lightProps = {};
	lightProps.Lights = {
		Vector4(1.0f, -1.0f, 0.0f, 0.0f), //direction
		Vector4(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f //color
	};
	lightProps.ViewVector = viewVector;
	lightProps.Intensity = 5.0f;
    lightProps.T = Matrix(
        0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f);

	Game->context->UpdateSubresource(constBuffer, 0, nullptr, &lightProps, 0, 0);
}

void LightComponent::Draw()
{
	Game->context->VSSetConstantBuffers(1, 1, &constBuffer);
	Game->context->PSSetConstantBuffers(1, 1, &constBuffer);
}

void LightComponent::DestroyResources()
{
	constBuffer->Release();

	Game = nullptr;

	GameComponent::DestroyResources();
}


Matrix LightComponent::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
{
    const auto proj = Matrix::CreatePerspectiveFieldOfView(
        Camera::Current->FOV, Camera::Current->AspectRatio, nearPlane,
        farPlane);
    const auto corners = Camera::Current->GetFrustumCornersWorldSpace(Camera::Current->GetViewMatrix(), proj);

    auto center = Vector3::Zero;
    for (const auto& v : corners)
    {
        center += Vector3(v);
    }
    center /= static_cast<float>(corners.size());

    const auto lightView = Matrix::CreateLookAt(center, center + Vector4(1.0f, -1.0f, 0.0f, 0.0f), Vector3::Up);

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

Vector4 LightComponent::GetShadowCascadeDistances() const
{
    return Vector4(shadowCascadeLevels_[0], shadowCascadeLevels_[1], shadowCascadeLevels_[2], shadowCascadeLevels_[3]);
}

std::vector<Matrix> LightComponent::GetLightSpaceMatrices()
{
    std::vector<Matrix> ret;
    for (size_t i = 0; i < shadowCascadeLevels_.size() + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(GetLightSpaceMatrix(Camera::Current->ClipPlanes.Near, shadowCascadeLevels_[i]));
        }
        else if (i < shadowCascadeLevels_.size())
        {
            ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels_[i - 1], shadowCascadeLevels_[i]));
        }
        else
        {
            ret.push_back(GetLightSpaceMatrix(shadowCascadeLevels_[i - 1], Camera::Current->ClipPlanes.Far));
        }
    }
    return ret;
}
