#pragma once

#include <map>

#include "MinimalCore.h"
#include "DirectXTK/DDSTextureLoader.h"


class Game;

struct Shader
{
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
};

class ShaderManager
{
public:
	struct Texture
	{
		ID3D11Resource* buffer;
		ID3D11ShaderResourceView* view;
	};

	static ShaderManager* Instance;

	ShaderManager();
	void ApplyShader(std::string shaderName);
	ID3D11ShaderResourceView* GetTextureView(std::string name);


private:
	Game* game;
	std::map<std::string, Shader> shaders;
	std::map<std::string, Texture> textures;


	void CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
};

