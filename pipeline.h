#pragma once
//pipeline.h__________________________________

#include "objects.h"


class pipeline{
public:
	HWND hWnd;
	IDXGISwapChain *SwapChain;             // the pointer to the swap chain interface
	ID3D11Device *Device;                     // the pointer to our Direct3D device interface
	ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
	ID3D11RenderTargetView *BackBuffer;    // global declaration
	ID3D11VertexShader *pVS;    // the vertex shader
	ID3D11PixelShader *pPS;     // the pixel shader
	ID3D11VertexShader *pVSTex;    // the vertex shader
	ID3D11PixelShader *pPSTex;     // the pixel shader
	ID3D11SamplerState *pSampler; // the sampler
	ID3D11Buffer *cb;
	ID3D11InputLayout *InputLayout;
	D3D11_MAPPED_SUBRESOURCE constbuffmvp; //for mapping const buffer of mvp

	ID3D10Blob *VS, *PS, *VSTex, *PSTex, *bloberror;

	CONSTBUFFWVP constbuffshader; // constant buffer

	//Textures
	ID3D11ShaderResourceView *pShaderRVBricks; //shader resource view
	ID3D11ShaderResourceView *pShaderRV0; //shader resource view
	ID3D11ShaderResourceView *pShaderRV1; //shader resource view
	ID3D11ShaderResourceView *pShaderRV2; //shader resource view
	ID3D11ShaderResourceView *pShaderRV3; //shader resource view
	ID3D11ShaderResourceView *pShaderRV4; //shader resource view
	ID3D11ShaderResourceView *pShaderRV5; //shader resource view
	ID3D11ShaderResourceView *pShaderRV6; //shader resource view
	ID3D11ShaderResourceView *pShaderRV7; //shader resource view
	ID3D11ShaderResourceView *pShaderRV8; //shader resource view
	ID3D11ShaderResourceView *pShaderRV9; //shader resource view
	ID3D11ShaderResourceView *shiptexture; //shader resource view

	pipeline(HWND hWndin);

	void InitDevSwap();
	void InitShaders();
	void InitConstBuff();
	void CreateVB(objectmesh &objectmeshpara);
};