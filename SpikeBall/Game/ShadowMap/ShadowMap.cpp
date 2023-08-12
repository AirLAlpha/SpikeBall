/**********************************************
 *
 *  ShadowMap.cpp
 *  �V���h�E�}�b�v
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "ShadowMap.h"
#include "DirectXHelpers.h"

#include "Light/DirectionalLight/DirectionalLight.h"

using namespace DirectX;

//	�R���X�g���N�^
ShadowMap::ShadowMap(DX::DeviceResources* deviceResources, UINT resolution)
{
	//	�f�o�C�X�̎擾
	auto device = deviceResources->GetD3DDevice();

	//	�[�x�o�b�t�@�Ɏg�p����t�H�[�}�b�g
	const DXGI_FORMAT DEPTH_FORMAT = DXGI_FORMAT_D16_UNORM;

	//	�[�x�o�b�t�@�p�e�N�X�`���̍쐬
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
	//	�[�x�r���[�̍쐬
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = DEPTH_FORMAT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		DX::ThrowIfFailed(
			device->CreateDepthStencilView(m_depthStencilTex.Get(), &desc, m_depthStencilView.ReleaseAndGetAddressOf())
		);
	}


	//	Z�l�e�N�X�`���Ɏg�p����t�H�[�}�b�g
	const DXGI_FORMAT TEX_FORMAT = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//	Z�l�e�N�X�`��
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

	//	Z�l�����_�[�^�[�Q�b�g�r���[
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = TEX_FORMAT;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		DX::ThrowIfFailed(
			device->CreateRenderTargetView(m_zTexture.Get(), &desc, m_zRTV.ReleaseAndGetAddressOf())
		);
	}

	//	Z�l�V�F�[�_�[���\�[�X�r���[
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

	//	�r���[�|�[�g�̍쐬
	{
		m_viewPort.TopLeftX = 0.0f;
		m_viewPort.TopLeftY = 0.0f;
		m_viewPort.Width = static_cast<FLOAT>(resolution);
		m_viewPort.Height = static_cast<FLOAT>(resolution);
		m_viewPort.MinDepth = 0.0f;
		m_viewPort.MaxDepth = 1.0f;
	}

	//	�X�N���[���̃r���[��ێ����Ă���
	m_screenRTV = deviceResources->GetRenderTargetView();
	m_screenDSV = deviceResources->GetDepthStencilView();
	m_screenViewPort = deviceResources->GetScreenViewport();


	//	�V���h�E�}�b�v�p�G�t�F�N�g�̍쐬
	m_shadowMapEffect = std::make_unique<ShadowMapEffect>(device);
	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionNormalTangentColorTexture>(device, m_shadowMapEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

//	�f�X�g���N�^
ShadowMap::~ShadowMap()
{
}


//--------------------------------------------------------------------------------
// �V���h�E�}�b�v�ɕ`��J�n
//--------------------------------------------------------------------------------
void ShadowMap::Begin(ID3D11DeviceContext* context, CommonStates* commonStates)
{
	//	�ݒ肳��Ă���X�e�[�g���N���A
	context->ClearState();

	//	�����_�[�^�[�Q�b�g�r���[�̃N���A
	context->ClearRenderTargetView(m_zRTV.Get(), DirectX::Colors::White);
	//	�[�x�X�e���V���r���[�̃N���A
	context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//	�`��X�e�[�g��ݒ�
	context->RSSetState(commonStates->CullNone());
	//	�[�x�o�b�t�@���g�p����p�ɐݒ�
	context->OMSetDepthStencilState(commonStates->DepthDefault(), 0xFFFFFFFF);

	//	�`��Ώۂ�ύX
	context->OMSetRenderTargets(1, m_zRTV.GetAddressOf(), m_depthStencilView.Get());

	//	�r���[�|�[�g��ύX
	context->RSSetViewports(1, &m_viewPort);

}

//--------------------------------------------------------------------------------
// �V���h�E�}�b�v�ɕ`��
//--------------------------------------------------------------------------------
void ShadowMap::RenderShadowMap(ID3D11DeviceContext* context, const SimpleMath::Matrix& world)
{
	//	�C���v�b�g���C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	//	�s��̐ݒ�
	m_shadowMapEffect->SetWorld(world);
	m_shadowMapEffect->SetView(m_light->GetLightView());
	m_shadowMapEffect->SetProjection(m_light->GetLightProjection());

	//	�V�F�[�_�[�̓K��
	m_shadowMapEffect->Apply(context);
}

//--------------------------------------------------------------------------------
// �V���h�E�}�b�v�ɕ`��I��
//--------------------------------------------------------------------------------
void ShadowMap::End(ID3D11DeviceContext* context)
{
	//	�`��Ώۂ�߂�
	context->OMSetRenderTargets(1, &m_screenRTV, m_screenDSV);

	//	�r���[�|�[�g��߂�
	context->RSSetViewports(1, &m_screenViewPort);

	//	�V�F�[�_�[�̉���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
