#pragma once
//pipeline.h__________________________________

#include "objects.h"


class pipeline{
public:

	ID3D11BlendState *pBlendState;

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
	ID3D11ShaderResourceView *asttexture; //shader resource view
	ID3D11ShaderResourceView *projtexture; //shader resource view
	ID3D11ShaderResourceView *shiptexture;
	ID3D11ShaderResourceView *lvl1back1texture;
	ID3D11ShaderResourceView *lvl1back2texture;
	ID3D11ShaderResourceView *lvl1back3texture;
	ID3D11ShaderResourceView *lvl1back4texture;
	ID3D11ShaderResourceView *gameovertexture;
	ID3D11ShaderResourceView *armourtexttexture;
	ID3D11ShaderResourceView *shieldtexttexture;
	ID3D11ShaderResourceView *text0texture;
	ID3D11ShaderResourceView *text1texture;
	ID3D11ShaderResourceView *text2texture;
	ID3D11ShaderResourceView *text3texture;
	ID3D11ShaderResourceView *shieldtexture;

	pipeline(HWND hWndin);

	void InitDevSwap();
	void InitShaders();
	void InitConstBuff();
	void CreateVB(objectmesh &objectmeshpara);
};