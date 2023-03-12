#pragma once

#include "../RendererComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class MeshComponent : public RendererComponent
{
public:
	MeshComponent(std::string fileNameModel, const wchar_t* fileNameTexture);

    void Initialize() override;

protected:
    LPCWSTR GetShaderName() override { return L"./Shaders/My3DShader.hlsl"; }

    std::string fNameModel;
    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

