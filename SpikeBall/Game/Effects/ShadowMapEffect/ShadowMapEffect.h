/**********************************************
 *
 *  ShadowMapEffect.h
 *  ShadowMapEffect.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#pragma once

class ShadowMapEffect : public DirectX::IEffect, public DirectX::IEffectMatrices
{
private:
	//	定数バッファ
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWVP;	// ワールドビュープロジェクション行列
	};

private:
	//	定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;

	//	頂点シェーダーのデータ
	std::vector<uint8_t> m_VSData;

	//	頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

private:
	//	ワールド行列
	DirectX::SimpleMath::Matrix		m_world;
	//	ビュー行列
	DirectX::SimpleMath::Matrix		m_view;
	//	射影行列
	DirectX::SimpleMath::Matrix		m_proj;

public:
	ShadowMapEffect(ID3D11Device* device);
	~ShadowMapEffect();


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

};

