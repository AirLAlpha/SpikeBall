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

class ShadowReciveEffect : public DirectX::IEffect
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

	//	シャドウマップテクスチャ
	ID3D11ShaderResourceView* m_shadowTexture;


public:
	ShadowReciveEffect(ID3D11Device* device);
	~ShadowReciveEffect();

	//	IEffect
	//	シェーダーの適応
	void Apply(ID3D11DeviceContext* context) override;
	//	頂点シェーダーの取得
	void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override;

public:
	//	行列の設定
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	//	カメラの行列を設定
	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_view = view; m_proj = proj; }
	//	ライトの行列を設定
	void SetLightMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_lightView = view; m_lightProj = proj; }

	//	ライトの方向を設定
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_lightDir = dir; }

	//	シャドウマップのテクスチャを設定
	void SetShadowMapTexture(ID3D11ShaderResourceView* texture) { m_shadowTexture = texture; }
};
