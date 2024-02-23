//  定数バッファ
cbuffer ConstBuffer : register(b0)
{
	float4x4    matWorld;           //  オブジェクトのワールド行列
	float4x4    matCamView;         //  カメラのビュー行列
	float4x4    matCamProj;         //  カメラの射影行列
	float4x4    matLightView;       //  ライトのビュー行列
	float4x4    matLightProj;       //  ライトの射影行列
	float4      lightDir;           //  ライトの向き
	float4      lightColor;         //  ライトの色
}

//  頂点入力
struct VS_INPUT
{
	float4 Position : SV_Position;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float4 Color : COLOR;
	float2 Texture : TEXCOORD;
};

//  ピクセル入力
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

