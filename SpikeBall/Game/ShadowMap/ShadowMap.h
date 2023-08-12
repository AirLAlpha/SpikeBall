/**********************************************
 *
 *  ShadowMap.h
 *  ShadowMap.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#pragma once

#include "Effects/ShadowMapEffect/ShadowMapEffect.h"

//	前方宣言
class DirectionalLight;


class ShadowMap
{
private:
	//	シャドウマップ用エフェクト
	std::unique_ptr<ShadowMapEffect>					m_shadowMapEffect;
	//	シャドウマップ用インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_inputLayout;

	//	深度バッファ用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_depthStencilTex;
	//	深度ビューの作成
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;

	//	Z値用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_zTexture;
	//	Z値用レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_zRTV;
	//	Z値用シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_zSRV;

	D3D11_VIEWPORT			m_viewPort;	//	シャドウマップのビューポート

private:
	ID3D11RenderTargetView* m_screenRTV;			//	スクリーンのレンダーターゲットビュー
	ID3D11DepthStencilView* m_screenDSV;			//	スクリーンの深度ステンシルビュー
	D3D11_VIEWPORT			m_screenViewPort;		//	スクリーンのビューポート

	DirectionalLight*		m_light;				//	描画に使用するライト

public:
	ShadowMap(DX::DeviceResources* deviceResources, UINT resolution);
	~ShadowMap();

	//	シャドウマップに描画開始
	void Begin(ID3D11DeviceContext* context, DirectX::CommonStates* commonStates);
	//	シャドウマップに描画
	void RenderShadowMap(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world);
	//	シャドウマップに描画終了
	void End(ID3D11DeviceContext* context);


public:
	//	シャドウマップテクスチャの取得
	ID3D11ShaderResourceView* GetShadowMapTexture() { return m_zSRV.Get(); }
	ID3D11Texture2D* GetShadowMapRawTexture()		{ return m_zTexture.Get(); }

public:
	//	ライトの設定
	void SetLight(DirectionalLight* light) { m_light = light; }

};
