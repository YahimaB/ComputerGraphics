#include "MeshComponent.h"

MeshComponent::MeshComponent(std::string modelName, std::string textureName) : RendererComponent()
{
    ModelName = modelName;
    TextureName = textureName;
}

void MeshComponent::Initialize()
{
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(ModelName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_FixInfacingNormals);

    std::cout << "NewModel:" << ModelName << std::endl;

    ProcessNode(pScene->mRootNode, pScene);

    RendererComponent::Initialize();
}

bool MeshComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
    std::cout << "newNode:" << node->mName.C_Str() << std::endl;

    aiVector3D* pos = new aiVector3D(5.0f, 3.0f, 4.0f);
    aiVector3D* scale = new aiVector3D();
    aiQuaternion* rot = new aiQuaternion();

    node->mTransformation.Decompose(*scale, *rot, *pos);
    std::cout << "translate:" << pos->x << " | " << pos->y << " | " << pos->z << std::endl;
    std::cout << "rot:" << rot->x << " | " << rot->y << " | " << rot->z << std::endl;
    //std::cout << "translate:" << pos->x << " | " << pos->y << " | " << pos->z << std::endl;

    for (UINT i = 0; i < node->mNumMeshes; i++)
    {
        int index = node->mMeshes[i];
        aiMesh* mesh = scene->mMeshes[index];
        std::cout << "mesh: " << index << std::endl;
        if (index <= 0)
            continue;

        ProcessMesh(mesh, scene, node->mTransformation);
        //return true;
    }
    for (UINT i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
    return false;
}

void MeshComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 trans)
{
    int vCount = points.size();

    for (UINT i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex point;

        aiVector3D vert = mesh->mVertices[i];
        //trans.

        point.pos.x = mesh->mVertices[i].x;
        point.pos.y = mesh->mVertices[i].y;
        point.pos.z = mesh->mVertices[i].z;
        point.pos.w = 1.0f;

        if (mesh->mTextureCoords[0])
        {
            point.tex.x = mesh->mTextureCoords[0][i].x;
            point.tex.y = mesh->mTextureCoords[0][i].y;
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
            indices.push_back(vCount + face.mIndices[j]);
    }
}
