#include "ShapeComponent.h"
#include "Game.h"

ShapeComponent::ShapeComponent()
{
	
}

void ShapeComponent::Initialize()
{
	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(Vertex) * pointsCount;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * indicesCount;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&indexBufDesc, &indexData, &indexBuffer);
}

void ShapeComponent::Draw()
{
	ShaderManager::Instance->ApplyShader(ShaderName);

	UINT strides[] = { sizeof(Vertex) };
	UINT offsets[] = { 0 };

	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, &vertexBuffer, strides, offsets);

	game->context->DrawIndexed(indicesCount, 0, 0);
}
