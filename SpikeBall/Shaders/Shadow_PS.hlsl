#include "Shadow_Header.hlsli"

//	ベーステクスチャ
Texture2D baseTex : register(t0);
//  Z値テクスチャ
Texture2D zTex : register(t1);
//  テクスチャサンプラ
SamplerState sam : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	//	ベーステクスチャの取得
    float4 col = baseTex.Sample(sam, input.Texture);
	
    //float4 col = lightColor;

	//  ライト目線のZ値を算出
	float z = input.CalculatablePos.z / input.CalculatablePos.w;

	// 射影空間のXY座標をテクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + input.LightViewPos.x / input.LightViewPos.w) * 0.5f;
	TransTexCoord.y = (1.0f - input.LightViewPos.y / input.LightViewPos.w) * 0.5f;

	//  テクスチャの範囲外にいるときは描画しない
	if(TransTexCoord.x < 0.001f || 0.999f < TransTexCoord.x ||
		TransTexCoord.y < 0.001f || 0.999f < TransTexCoord.y)
		return col;
	
	// テクスチャからZ値抽出
	float SM_Z = zTex.Sample(sam, TransTexCoord).x;
	
	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if (z > SM_Z + 0.005f)
	{
		col.rgb = col.rgb * 0.5f;  
	}
	

	//  ランバート拡散
    float4 lDir = normalize(lightDir);
    float4 normal = normalize(input.Normal);
    float t = dot(normal, lDir);
    t = min(1.0f, t);
    col.rgb = col.rgb * t;
	
	return col;
}