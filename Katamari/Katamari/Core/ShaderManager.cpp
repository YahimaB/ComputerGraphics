#include "ShaderManager.h"
#include "Game.h"

ShaderManager* ShaderManager::Instance = 0;

ShaderManager::ShaderManager()
{
	game = Game::Instance;
	Instance = this;
}

void ShaderManager::ApplyShader(std::string shaderName)
{
    Shader shader;

    auto shaderPair = shaders.find(shaderName);
    if (shaderPair != shaders.end()) {

        shader = shaderPair->second;
    }
    else {
		shader = {};
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* inputLayout;

		ID3DBlob* vsBlob;
		{
			ShaderManager::CompileShaderFromFile(shaderName, nullptr, "VSMain", "vs_5_0", &vsBlob);

			game->device->CreateVertexShader(
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				nullptr, &vertexShader);
		}

		ID3DBlob* psBlob;
		{
			D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

			ShaderManager::CompileShaderFromFile(shaderName, Shader_Macros, "PSMain", "ps_5_0", &psBlob);
			game->device->CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr, &pixelShader);
			psBlob->Release();
		}

		{
			D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			game->device->CreateInputLayout(
				inputElementDesc,
				ARRAYSIZE(inputElementDesc),
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				&inputLayout);

			vsBlob->Release();
		}
		
		shader.vShader = vertexShader;
		shader.pShader = pixelShader;
		shader.inputLayout = inputLayout;

		shaders[shaderName] = shader;
    }

    game->context->IASetInputLayout(shader.inputLayout);
    game->context->VSSetShader(shader.vShader, nullptr, 0);
    game->context->PSSetShader(shader.pShader, nullptr, 0);
}

void ShaderManager::CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	auto wideStrName = std::wstring(shaderName.begin(), shaderName.end());

    ID3DBlob* pErrorBlob;
    auto res = D3DCompileFromFile(wideStrName.c_str(),
        shaderMacros /*macros*/,
        nullptr /*include*/,
        entryPoint, 
		shaderModel,
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, 
        blobOut,
        &pErrorBlob);


	if (FAILED(res)) {
		if (pErrorBlob) {
			char* compileErrors = (char*)(pErrorBlob->GetBufferPointer());
			std::cout << compileErrors << std::endl;
		}
		else
		{
			std::cout << "Missing shader file: " << shaderName << std::endl;
		}
	}
}

ID3D11ShaderResourceView* ShaderManager::GetTextureView(std::string name)
{
	if (textures.find(name) != textures.end())
	{
		return textures[name].view;
	}

	std::wstring wideStr = std::wstring(name.begin(), name.end());
	auto res = DirectX::CreateDDSTextureFromFile(game->device, wideStr.c_str(), &textures[name].buffer, &textures[name].view);

	game->context->GenerateMips(textures[name].view);

	return textures[name].view;
}
