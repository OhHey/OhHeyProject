//pipeline.cpp__________________________________
#include "pipeline.h"


pipeline::pipeline(HWND hWndin):hWnd(hWndin)
{
	InitDevSwap();
	InitShaders();
	InitConstBuff();
}

void pipeline::InitDevSwap(){
	HRESULT error;
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	error = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&SwapChain,
		&Device,
		NULL,
		&devcon);

	if (FAILED(error)) { MessageBox(NULL,L"OH NOES SWAPCHAIN",L"QUIT",NULL); }

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	Device->CreateRenderTargetView(pBackBuffer, NULL, &BackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	devcon->OMSetRenderTargets(1, &BackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}

void pipeline::InitShaders(){
	HRESULT error;

	// load and compile the two shaders
	
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, &PS, 0);
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "VShaderTex", "vs_5_0", 0, 0, &VSTex, &bloberror);
	D3DCompileFromFile(L"shaders.hlsl", 0, 0, "PShaderTex", "ps_5_0", 0, 0, &PSTex, 0);
	//char* errortext = (char*)bloberror->GetBufferPointer();

	// encapsulate both shaders into shader objects
	error = Device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES VS CREATE!",L"QUIT",NULL); }
	error = Device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES PS CREATE!",L"QUIT",NULL); }
	error = Device->CreateVertexShader(VSTex->GetBufferPointer(), VSTex->GetBufferSize(), NULL, &pVSTex);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES VSTEX CREATE!",L"QUIT",NULL); }
	error = Device->CreatePixelShader(PSTex->GetBufferPointer(), PSTex->GetBufferSize(), NULL, &pPSTex);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES PSTEXT CREATE!",L"QUIT",NULL); }

	//load the picture to a shader resource view
	error = CreateWICTextureFromFile(Device, devcon, L"bricks.JPEG",nullptr,&pShaderRVBricks);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER BRICKS!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"ship.png",nullptr,&shiptexture);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER SHIP",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"0.PNG",nullptr,&pShaderRV0);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 0!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"1.PNG",nullptr,&pShaderRV1);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 1!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"2.PNG",nullptr,&pShaderRV2);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 2!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"3.PNG",nullptr,&pShaderRV3);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 3!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"4.PNG",nullptr,&pShaderRV4);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 4!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"5.PNG",nullptr,&pShaderRV5);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 5!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"6.PNG",nullptr,&pShaderRV6);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 6!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"7.PNG",nullptr,&pShaderRV7);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 7!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"8.PNG",nullptr,&pShaderRV8);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 8!",L"QUIT",NULL); }
	error = CreateWICTextureFromFile(Device, devcon, L"9.PNG",nullptr,&pShaderRV9);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES WICLOADER 9!",L"QUIT",NULL); }

	//sampler description
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 0;
	// Specify how texture coordinates outside of the range 0..1 are resolved.
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	// Use no special MIP clamping or bias.
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Don't use a comparison function.
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	// Border address mode is not used, so this parameter is ignored.
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;

	//make the sampler
	error = Device->CreateSamplerState(&samplerDesc,&pSampler);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES SAMPLER!",L"QUIT",NULL); }

	//set shader resource to that made from picture and set sampler
	
	devcon->PSSetSamplers(0,1,&pSampler);

	//set inputlayouy

	D3D11_INPUT_ELEMENT_DESC InputDesc[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	error = Device->CreateInputLayout(InputDesc, 2, VSTex->GetBufferPointer(), VSTex->GetBufferSize(), &InputLayout);
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES INPUTLAYOUT iedtex!",L"QUIT",NULL); }

	devcon->IASetInputLayout(InputLayout);

}

void pipeline::InitConstBuff()
{
	HRESULT error;
	//Constant buffer description - for WVP matrix transfer
	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(cbDesc));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = sizeof(CONSTBUFFWVP);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	//Make the constant buffer
	error = Device->CreateBuffer(&cbDesc, NULL, &cb);       // create the buffer
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES CONST BUFF!",L"QUIT",NULL); }

	//Set the constant buffer
	devcon->VSSetConstantBuffers( 0, 1, &cb );
}

void pipeline::CreateVB(objectmesh &objectmeshpara)
{
	HRESULT error;
	error = Device->CreateBuffer(&objectmeshpara.BufferDesc, NULL, &objectmeshpara.VertexBuffer);       // create the buffer
	if (FAILED(error)) { MessageBox(NULL,L"OH NOES SQRTEX VB!",L"QUIT",NULL); }

	D3D11_MAPPED_SUBRESOURCE mapsqrtex;
	devcon->Map(objectmeshpara.VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapsqrtex);		// map the buffer
	memcpy(mapsqrtex.pData, objectmeshpara.vertices, sizeof(objectmeshpara.vertices));							// copy the data
	devcon->Unmap(objectmeshpara.VertexBuffer, NULL);	
}