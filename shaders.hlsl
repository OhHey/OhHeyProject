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
	//MipFilter = NONE;
    Filter = MIN_MAG_LINEAR_MIP_POINT;
	//ComparisonFunc = LESS;
	//ComparisonFilter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
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
	//if(sampled.a == 0.0){sampled.rgb = 0.0;}
	//return sampled;
    //return float4(bricks.Sample(gogosampler,texcoord).aaa,1.0);
}

