#include "OldShaderManager.h"
#include "Game.h"

OldShaderManager* OldShaderManager::Instance = 0;

OldShaderManager::OldShaderManager()
{
	game = Game::Instance;
	Instance = this;
}

void OldShaderManager::ApplyShader(std::string shaderName)
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
			OldShaderManager::CompileShaderFromFile(shaderName, nullptr, "VSMain", "vs_5_0", &vsBlob);

			game->device->CreateVertexShader(
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				nullptr, &vertexShader);
		}

		ID3DBlob* psBlob;
		{
			D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };

			OldShaderManager::CompileShaderFromFile(shaderName, Shader_Macros, "PSMain", "ps_5_0", &psBlob);
			game->device->CreatePixelShader(
				psBlob->GetBufferPointer(),
				psBlob->GetBufferSize(),
				nullptr, &pixelShader);
			psBlob->Release();
		}

		/*{
			D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
			{
				D3D11_INPUT_ELEMENT_DESC {
					"POSITION",
					0,
					DXGI_FORMAT_R32G32B32A32_FLOAT,
					0,
					0,
					D3D11_INPUT_PER_VERTEX_DATA,
					0},
				D3D11_INPUT_ELEMENT_DESC {
					"TEXCOORD",
					0,
					DXGI_FORMAT_R32G32B32A32_FLOAT,
					0,
					D3D11_APPEND_ALIGNED_ELEMENT,
					D3D11_INPUT_PER_VERTEX_DATA,
					0},
				D3D11_INPUT_ELEMENT_DESC {
					"NORMAL",
					0,
					DXGI_FORMAT_R32G32B32A32_FLOAT,
					0,
					D3D11_APPEND_ALIGNED_ELEMENT,
					D3D11_INPUT_PER_VERTEX_DATA,
					0}
			};

			game->device->CreateInputLayout(
				inputElementDesc,
				ARRAYSIZE(inputElementDesc),
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				&inputLayout);

			vsBlob->Release();
		}*/

		CreateInputLayoutDescFromVertexShaderSignature(vsBlob, game->device, &inputLayout);
		vsBlob->Release();

		shader.vShader = vertexShader;
		shader.pShader = pixelShader;
		shader.inputLayout = inputLayout;

		shaders[shaderName] = shader;
	}

	game->context->IASetInputLayout(shader.inputLayout);
	game->context->VSSetShader(shader.vShader, nullptr, 0);

	game->context->PSSetShader(shader.pShader, nullptr, 0);
}

void OldShaderManager::CompileShaderFromFile(std::string shaderName, D3D_SHADER_MACRO shaderMacros[], LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
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

ID3D11ShaderResourceView* OldShaderManager::GetTextureView(std::string name)
{
	if (textures.find(name) != textures.end())
	{
		return textures[name].view;
	}

	std::wstring wideStr = std::wstring(name.begin(), name.end());
	auto res = DirectX::CreateDDSTextureFromFile(game->device, wideStr.c_str(), &textures[name].buffer, &textures[name].view);

	//game->context->GenerateMips(textures[name].view);

	return textures[name].view;
}

HRESULT OldShaderManager::CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT32 i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}
