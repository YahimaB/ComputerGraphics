#pragma once

#include <map>

#include "../MinimalCore.h"
#include "../DirectXTK/DDSTextureLoader.h"

enum EShaderType
{
	ST_Vertex = 1 << 0,
	ST_Pixel = 1 << 1,
	ST_Geometry = 1 << 2,
	ST_Compute = 1 << 3,
};
DEFINE_ENUM_FLAG_OPERATORS(EShaderType)

struct FShaderData
{
	std::string ShaderName;
	EShaderType Type;
};

struct FShader
{
	ID3D11VertexShader* VShader = nullptr;
	ID3D11PixelShader* PShader = nullptr;
	ID3D11GeometryShader* GShader = nullptr;
	ID3D11ComputeShader* CShader = nullptr;
};

struct Texture
{
	ID3D11Resource* buffer;
	ID3D11ShaderResourceView* view;
};

class ShaderManager
{
public:
	static ShaderManager* Instance;

public:
	ShaderManager();

	void InitShader(FShaderData data);
	void SetShader(FShaderData data);
	ID3D11ShaderResourceView* GetTextureView(std::string name);

private:
	Game* game;
	ID3D11InputLayout* inputLayout = nullptr;

	std::map<std::string, FShader> shaders;
	std::map<std::string, Texture> textures;

private:
	void CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut);
	HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
};

