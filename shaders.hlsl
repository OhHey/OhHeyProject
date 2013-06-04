struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct VOutTex
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};



SamplerState gogosampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};


cbuffer cb
{
    row_major MATRIX WVP;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;
	position.w = 1.0f;
    output.position = mul(position, WVP);
    output.color = color;

    return output;
}

VOutTex VShaderTex(float4 position : POSITION, float2 texcoord : TEXCOORD)
//VOutTex VShaderTex(float4 position : POSITION)
{
	VOutTex output;
	position.w = 1.0f;
	output.position = mul(position, WVP);
	output.texcoord = texcoord;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}

Texture2D bricks;

float4 PShaderTex(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
//float4 PShaderTex(float4 position : SV_POSITION) : SV_TARGET
{
	float4 colourtest = float4( 0, 1, 0, 1 );
	//return colourtest;
	
    return bricks.Sample(gogosampler,texcoord);
}

