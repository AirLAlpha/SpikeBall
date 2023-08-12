/**********************************************
 *
 *  ShadowReciveEffect.cpp
 *  影を受け取るエフェクト
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/04
 *
 **********************************************/
#include "pch.h"
#include "ShadowReciveEffect.h"

#include "Utilities/ReadData.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;


//	コンストラクタ
ShadowReciveEffect::ShadowReciveEffect(ID3D11Device* device)
{
	//	シェーダーの読み込み
	m_VSData = DX::ReadData(L"Resources/Shaders/Shadow_VS.cso");
	std::vector<uint8_t> PSData = DX::ReadData(L"Resources/Shaders/Shadow_PS.cso");

	//	頂点シェーダーの作成
	DX::ThrowIfFailed(
		device->CreateVertexShader(m_VSData.data(), m_VSData.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);
	//	ピクセルシェーダーの作成
	DX::ThrowIfFailed(
		device->CreatePixelShader(PSData.data(), PSData.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	定数バッファの作成
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = GetConstantBufferSize<ConstBuffer>();
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = FALSE;
	device->CreateBuffer(&desc, nullptr, m_cBuffer.ReleaseAndGetAddressOf());
}

//	デストラクタ
ShadowReciveEffect::~ShadowReciveEffect()
{
}

//--------------------------------------------------------------------------------
// シェーダーの適応
//--------------------------------------------------------------------------------
void ShadowReciveEffect::Apply(ID3D11DeviceContext* context)
{
	//	定数バッファの更新
	ConstBuffer buff = {};
	buff.matWorld = m_world.Transpose();
	buff.matView = m_view.Transpose();
	buff.matProj = m_proj.Transpose();
	buff.matLightView = m_lightView.Transpose();
	buff.matLightProj = m_lightProj.Transpose();

	SimpleMath::Vector4 lightDir = static_cast<SimpleMath::Vector4>(m_lightDir);
	lightDir.w = 1.0f;
	buff.lightDir = lightDir;
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &buff, 0, 0);

	//	定数バッファを設定
	context->VSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());

	//	テクスチャの設定
	context->PSSetShaderResources(0, 1, &m_shadowTexture);

	//	シェーダーを設定
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

//--------------------------------------------------------------------------------
// 頂点シェーダーの取得
//--------------------------------------------------------------------------------
void ShadowReciveEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	//	どちらかが nullptr ならエラー
	assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);

	//	情報を渡す
	*pShaderByteCode = m_VSData.data();
	*pByteCodeLength = m_VSData.size();
}


