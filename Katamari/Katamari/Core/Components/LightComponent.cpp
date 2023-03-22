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
	constBufPerSceneDesc.ByteWidth = sizeof(CBDataPerScene);

	Game->device->CreateBuffer(&constBufPerSceneDesc, nullptr, &constBuffer);
}

void LightComponent::Update(float deltaTime)
{
	Vector3 temp = Camera->Transform->GetModel().Backward();

	CBDataPerScene sceneData = {};
	sceneData.lightPos = Vector4(1.0f, 1.0f, 1.0f, 0.0f);
	sceneData.lightColorAmbStr = Vector4(1.0f, 1.0f, 1.0f, 0.4f);
	sceneData.viewDirSpecStr = Vector4(temp);
	sceneData.viewDirSpecStr.Normalize();
	sceneData.viewDirSpecStr.w = 0.5f;
	sceneData.lightPos.Normalize();

	Game->context->UpdateSubresource(constBuffer, 0, nullptr, &sceneData, 0, 0);
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
