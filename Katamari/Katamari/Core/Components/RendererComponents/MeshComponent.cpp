#include "MeshComponent.h"

MeshComponent::MeshComponent(std::string fileNameModel, const wchar_t* fileNameTexture) : RendererComponent()
{
    fNameModel = fileNameModel;
}

void MeshComponent::Initialize()
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(fNameModel, aiProcess_Triangulate);

    ProcessNode(pScene->mRootNode, pScene);

    RendererComponent::Initialize();
}

void MeshComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene);
    }
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

void MeshComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex point;

        point.pos.x = mesh->mVertices[i].x;
        point.pos.y = mesh->mVertices[i].y;
        point.pos.z = mesh->mVertices[i].z;
        point.pos.w = 1.0f;

        if (mesh->mTextureCoords[0])
        {
            point.col.x = mesh->mTextureCoords[0][i].x;
            point.col.y = mesh->mTextureCoords[0][i].y;
        }

        point.normal.x = mesh->mNormals[i].x;
        point.normal.y = mesh->mNormals[i].y;
        point.normal.z = mesh->mNormals[i].z;
        point.normal.w = 0.0f;

        points.push_back(point);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
}
