#pragma once

#include <map>

#include "MinimalCore.h"
#include "DirectXTK/DDSTextureLoader.h"


class Game;

template <typename T>
struct ShaderData
{
	T* Shader;

};

struct Shader
{
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11InputLayout* inputLayout;
};

class OldShaderManager
{
public:
	struct Texture
	{
		ID3D11Resource* buffer;
		ID3D11ShaderResourceView* view;
	};

	static OldShaderManager* Instance;

	OldShaderManager();
	void ApplyShader(std::string shaderName);
	ID3D11ShaderResourceView* GetTextureView(std::string name);


private:
	Game* game;
	std::map<std::string, Shader> shaders;
	std::map<std::string, Texture> textures;

	std::map<std::string, ShaderData<ID3D11DeviceChild>> tempShaders;


	void CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
	HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
};

