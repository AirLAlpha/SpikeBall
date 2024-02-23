//  �萔�o�b�t�@
cbuffer ConstBuffer : register(b0)
{
	float4x4    matWorld;           //  �I�u�W�F�N�g�̃��[���h�s��
	float4x4    matCamView;         //  �J�����̃r���[�s��
	float4x4    matCamProj;         //  �J�����̎ˉe�s��
	float4x4    matLightView;       //  ���C�g�̃r���[�s��
	float4x4    matLightProj;       //  ���C�g�̎ˉe�s��
	float4      lightDir;           //  ���C�g�̌���
	float4      lightColor;         //  ���C�g�̐F
}

//  ���_����
struct VS_INPUT
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float2 Texture : TEXCOORD;
};

//  �s�N�Z������
struct PS_INPUT
{
	float4 Pos              : SV_Position;
	float4 WorldPos         : TEXCOORD3;
	float4 CalculatablePos  : TEXCOORD;
	float4 LightViewPos     : TEXCOORD1;
	float4 Normal           : TEXCOORD2;
	float4 Color            : COLOR;
	float2 Texture          : TEXCOORD4;
};

