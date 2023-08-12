/**********************************************
 *
 *  ShadowMap.cpp
 *  シャドウマップ
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "ShadowMap.h"
#include "DirectXHelpers.h"

#include "Light/DirectionalLight/DirectionalLight.h"

using namespace DirectX;

//	コンストラクタ
ShadowMap::ShadowMap(DX::DeviceResources* deviceResources, UINT resolution)
{
	//	デバイスの取得
	auto device = deviceResources->GetD3DDevice();

	//	深度バッファに使用するフォーマット
	const DXGI_FORMAT DEPTH_FORMAT = DXGI_FORMAT_D16_UNORM;

	//	深度バッファ用テクスチャの作成
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = resolution;
		desc.Height = resolution;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DEPTH_FORMAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		DX::ThrowIfFailed(
			device->CreateTexture2D(&desc, NULL, m_depthStencilTex.ReleaseAndGetAddressOf())
		);
	}
	//	深度ビューの作成
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DEPTH_FORMAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		DX::ThrowIfFailed(
			device->CreateDepthStencilView(m_depthStencilTex.Get(), &desc, m_depthStencilView.ReleaseAndGetAddressOf())
		);
	}


	//	Z値テクスチャに使用するフォーマット
	const DXGI_FORMAT TEX_FORMAT = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//	Z値テクスチャ
	{
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = resolution;
		desc.Height = resolution;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = TEX_FORMAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		DX::ThrowIfFailed(
			device->CreateTexture2D(&desc, nullptr, m_zTexture.ReleaseAndGetAddressOf())
		);
	}

	//	Z値レンダーターゲットビュー
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = TEX_FORMAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		DX::ThrowIfFailed(
			device->CreateRenderTargetView(m_zTexture.Get(), &desc, m_zRTV.ReleaseAndGetAddressOf())
		);
	}

	//	Z値シェーダーリソースビュー
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = TEX_FORMAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MostDetailedMip = 0;
		desc.Texture2D.MipLevels = static_cast<UINT>(-1);

		DX::ThrowIfFailed(
			device->CreateShaderResourceView(m_zTexture.Get(), &desc, m_zSRV.ReleaseAndGetAddressOf())
		);
	}

	//	ビューポートの作成
	{
		m_viewPort.TopLeftX = 0.0f;
		m_viewPort.TopLeftY = 0.0f;
		m_viewPort.Width = static_cast<FLOAT>(resolution);
		m_viewPort.Height = static_cast<FLOAT>(resolution);
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;
	}

	//	スクリーンのビューを保持しておく
	m_screenRTV = deviceResources->GetRenderTargetView();
	m_screenDSV = deviceResources->GetDepthStencilView();
	m_screenViewPort = deviceResources->GetScreenViewport();


	//	シャドウマップ用エフェクトの作成
	m_shadowMapEffect = std::make_unique<ShadowMapEffect>(device);
	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionNormalTangentColorTexture>(device, m_shadowMapEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

//	デストラクタ
ShadowMap::~ShadowMap()
{
}


//--------------------------------------------------------------------------------
// シャドウマップに描画開始
//--------------------------------------------------------------------------------
void ShadowMap::Begin(ID3D11DeviceContext* context, CommonStates* commonStates)
{
	//	設定されているステートをクリア
	context->ClearState();

	//	レンダーターゲットビューのクリア
	context->ClearRenderTargetView(m_zRTV.Get(), DirectX::Colors::White);
	//	深度ステンシルビューのクリア
	context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//	描画ステートを設定
	context->RSSetState(commonStates->CullNone());
	//	深度バッファを使用する用に設定
	context->OMSetDepthStencilState(commonStates->DepthDefault(), 0xFFFFFFFF);

	//	描画対象を変更
	context->OMSetRenderTargets(1, m_zRTV.GetAddressOf(), m_depthStencilView.Get());

	//	ビューポートを変更
	context->RSSetViewports(1, &m_viewPort);

}

//--------------------------------------------------------------------------------
// シャドウマップに描画
//--------------------------------------------------------------------------------
void ShadowMap::RenderShadowMap(ID3D11DeviceContext* context, const SimpleMath::Matrix& world)
{
	//	インプットレイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	//	行列の設定
	m_shadowMapEffect->SetWorld(world);
	m_shadowMapEffect->SetView(m_light->GetLightView());
	m_shadowMapEffect->SetProjection(m_light->GetLightProjection());

	//	シェーダーの適応
	m_shadowMapEffect->Apply(context);
}

//--------------------------------------------------------------------------------
// シャドウマップに描画終了
//--------------------------------------------------------------------------------
void ShadowMap::End(ID3D11DeviceContext* context)
{
	//	描画対象を戻す
	context->OMSetRenderTargets(1, &m_screenRTV, m_screenDSV);

	//	ビューポートを戻す
	context->RSSetViewports(1, &m_screenViewPort);

	//	シェーダーの解除
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
