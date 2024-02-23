/**********************************************
 *
 *  ShadowReciveEffect.h
 *  ShadowReciveEffect.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/007/04
 *
 **********************************************/
#pragma once

//	前方宣言
class DirectionalLight;

class ShadowReceiveEffect : public DirectX::IEffect, public DirectX::IEffectMatrices
{
private:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Matrix		matLightView;
		DirectX::SimpleMath::Matrix		matLightProj;
		DirectX::SimpleMath::Vector4	lightDir;
		DirectX::SimpleMath::Vector4	lightColor;
	};

private:
	//	定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;

	//	頂点シェーダーデーター
	std::vector<uint8_t> m_VSData;

	//	頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

private:
	//	オブジェクトのワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//	カメラのビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//	カメラの射影行列
	DirectX::SimpleMath::Matrix m_proj;
	//	ライトのビュー行列
	DirectX::SimpleMath::Matrix m_lightView;
	//	ライトの射影行列
	DirectX::SimpleMath::Matrix m_lightProj;

	//	ライトの方向
	DirectX::SimpleMath::Vector3 m_lightDir;
	//	ライトの色
	DirectX::SimpleMath::Vector4 m_lightColor;

	//	ベーステクスチャ
	ID3D11ShaderResourceView* m_baseTexture;
	//	シャドウマップテクスチャ
	ID3D11ShaderResourceView* m_shadowTexture;


public:
	ShadowReceiveEffect(ID3D11Device* device);
	~ShadowReceiveEffect();

	//	IEffect
	//	シェーダーの適応
	void Apply(ID3D11DeviceContext* context) override;
	//	頂点シェーダーの取得
	void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override;

	//	IEffectMatricies
	void __vectorcall SetWorld(const DirectX::FXMMATRIX world) override { m_world = world; }
	void __vectorcall SetView(const DirectX::FXMMATRIX view) override { m_view = view; }
	void __vectorcall SetProjection(const DirectX::FXMMATRIX proj) override { m_proj = proj; }
	void __vectorcall SetMatrices(const DirectX::FXMMATRIX world, const DirectX::FXMMATRIX view, const DirectX::FXMMATRIX proj)
	{
		m_world = world;
		m_view = view;
		m_proj = proj;
	}

public:		//	Setter
	//	ライトの行列を設定
	void SetLightMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_lightView = view; m_lightProj = proj; }
	//	ライトの方向を設定
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_lightDir = dir; }
	//	ベーステクスチャの設定
	void SetBaseTexture(ID3D11ShaderResourceView* texture) { m_baseTexture = texture; }
	//	シャドウマップのテクスチャを設定
	void SetShadowMapTexture(ID3D11ShaderResourceView* texture) { m_shadowTexture = texture; }
	//	ライトの色を設定
	void SetLightColor(const DirectX::SimpleMath::Vector4& color) { m_lightColor = color; }
};
