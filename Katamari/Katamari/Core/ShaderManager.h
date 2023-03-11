#pragma once

#include <map>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

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
	static ShaderManager* Instance;

	ShaderManager();
	void ApplyShader(LPCWSTR shaderName);


private:
	Game* game;
	std::map<LPCWSTR, Shader> shaders;

	void CompileShaderFromFile(LPCWSTR shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
};

