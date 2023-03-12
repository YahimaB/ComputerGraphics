#pragma once

#include "../RendererComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class MeshComponent : public RendererComponent
{
public:
	MeshComponent(std::string modelName, std::string textureName);

    void Initialize() override;

protected:
    std::string ModelName;
    std::string TextureName;

    void ProcessNode(aiNode* node, const aiScene* scene);
    void ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::string GetTextureName() override { return TextureName; }
};

