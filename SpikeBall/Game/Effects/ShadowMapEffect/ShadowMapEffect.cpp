/**********************************************
 *
 *  ShadowMapEffect.cpp
 *  シャドウマップ用エフェクト
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "ShadowMapEffect.h"
#include "Utilities/ReadData.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

//	コンストラクタ
ShadowMapEffect::ShadowMapEffect(ID3D11Device* device)
{
	//	シェーダーの読み込み
	m_VSData = DX::ReadData(L"Resources/Shaders/ShadowMap_VS.cso");
	std::vector<uint8_t> PSData = DX::ReadData(L"Resources/Shaders/ShadowMap_PS.cso");

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
ShadowMapEffect::~ShadowMapEffect()
{
}


//--------------------------------------------------------------------------------
// シェーダーの適応
//--------------------------------------------------------------------------------
void ShadowMapEffect::Apply(ID3D11DeviceContext* context)
{
	//	定数バッファの更新
	ConstBuffer buff = {};
	SimpleMath::Matrix WVP = m_world * m_view * m_proj;
	buff.matWVP = WVP.Transpose();
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &buff, 0, 0);

	//	定数バッファを設定
	context->VSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());

	//	シェーダーを設定
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

//--------------------------------------------------------------------------------
// 頂点シェーダーの取得
//--------------------------------------------------------------------------------
void ShadowMapEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	//	どちらかが nullptr ならエラー
	assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);

	//	情報を渡す
	*pShaderByteCode = m_VSData.data();
	*pByteCodeLength = m_VSData.size();
}
