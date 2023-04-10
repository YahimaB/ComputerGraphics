#include "RendererComponent.h"
#include "LightComponent.h"

RendererComponent::RendererComponent()
{
	Game = ::Game::Instance;
	Camera = ::Camera::Current;
}

void RendererComponent::Initialize()
{
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * points.size();

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Game->device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(ObjectData);

	Game->device->CreateBuffer(&constBufDesc, NULL, &constObjectBuffer);

	D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Game->device->CreateSamplerState(&samplerStateDesc, &samplerState);

	ShaderManager::Instance->InitShader(GetBaseShader());
	ShaderManager::Instance->InitShader(GetShadowShader());
}

void RendererComponent::Update(float deltaTime)
{
	Vector3 scale, pos;
	Quaternion rot;

	Matrix world = Transform->GetModel();
	world.Decompose(scale, rot, pos);

	ObjectData objData = {};
	objData.World = world;
	objData.WorldView = world * Camera->GetViewMatrix();
	objData.World_View_Projection = world * Camera->GetViewProjectionMatrix();
	objData.invTrWorld = (Matrix::CreateScale(scale) * Matrix::CreateFromQuaternion(rot)).Invert().Transpose();

	Game->context->UpdateSubresource(constObjectBuffer, 0, nullptr, &objData, 0, 0);
}

void RendererComponent::PrepareFrame()
{
	ShaderManager::Instance->SetShader(GetShadowShader());

	Game->context->IASetPrimitiveTopology(GetTopology());
	Game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

	Game->context->VSSetConstantBuffers(0, 1, &constObjectBuffer);

	Game->context->DrawIndexed(indices.size(), 0, 0);
}

void RendererComponent::Draw()
{
	ShaderManager::Instance->SetShader(GetBaseShader());

	Game->context->IASetPrimitiveTopology(GetTopology());
	Game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

	Game->context->VSSetConstantBuffers(0, 1, &constObjectBuffer);
	Game->context->PSSetConstantBuffers(0, 1, &constObjectBuffer);

	ID3D11ShaderResourceView* test = ShaderManager::Instance->GetTextureView(GetTextureName());
	Game->context->PSSetShaderResources(0, 1, &test);
	Game->context->PSSetShaderResources(1, 1, &Game->shadowSRV);

	Game->context->PSSetSamplers(0, 1, &samplerState);

	Game->context->DrawIndexed(indices.size(), 0, 0);
}

void RendererComponent::DestroyResources()
{
	points.clear();
	indices.clear();

	vertexBuffer->Release();
	indexBuffer->Release();
	constObjectBuffer->Release();

	Game = nullptr;

	GameComponent::DestroyResources();
}
