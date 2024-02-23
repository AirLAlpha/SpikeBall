#include "Shadow_Header.hlsli"

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT) 0;

	//  �J�����ڐ�
	float4 pos = mul(input.Position, matWorld);
	output.WorldPos = pos;
	pos = mul(pos, matCamView);
	pos = mul(pos, matCamProj);
	output.Pos = pos;
	
	//  ���C�g�ڐ�
	pos = mul(input.Position, matWorld);
	pos = mul(pos, matLightView);
	pos = mul(pos, matLightProj);
	output.LightViewPos = pos;
	output.CalculatablePos = pos;
	
	matrix w = matWorld;
	w._41_42_43_44 = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 nor = float4(input.Normal, 1.0f);
	nor = mul(nor, w);
	nor.w = 1.0f;
	output.Normal = nor;
	
    output.Texture = input.Texture;
	
	return output;
}