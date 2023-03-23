#include "LightComponent.h"

LightComponent::LightComponent()
{
	Game = ::Game::Instance;
	Camera = ::Camera::Current;
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
		Vector4(1.0f, -1.0f, 0.0f, 0.0f),
		Vector4(1.0f, 1.0f, 1.0f, 1.0f) * 0.2f
	};
	lightProps.ViewVector = viewVector;
	lightProps.Intensity = 5.0f;

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
