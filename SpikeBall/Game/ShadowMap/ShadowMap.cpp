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

#include "DirectionalLight/DirectionalLight.h"

using namespace DirectX;

//	�R���X�g���N�^
ShadowMap::ShadowMap(const DX::DeviceResources& deviceResources, UINT resolution)
{
	//	�f�o�C�X�̎擾
	auto device = deviceResources.GetD3DDevice();

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
	//	Z�l�p�e�N�X�`���̍쐬
	m_renderTexture = std::make_unique<DX::RenderTexture>(TEX_FORMAT);
	m_renderTexture->SetDevice(device);
	m_renderTexture->SizeResources(resolution, resolution);

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
	m_screenRTV = deviceResources.GetRenderTargetView();
	m_screenDSV = deviceResources.GetDepthStencilView();
	m_screenViewPort = deviceResources.GetScreenViewport();


	//	�V���h�E�}�b�v�p�G�t�F�N�g�̍쐬
	m_shadowMapEffect = std::make_unique<ShadowMapEffect>(device);
	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionNormalTexture>(device, m_shadowMapEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

//	�f�X�g���N�^
ShadowMap::~ShadowMap()
{
}


//--------------------------------------------------------------------------------
// �V���h�E�}�b�v�ɕ`��J�n
//--------------------------------------------------------------------------------
void ShadowMap::Begin(ID3D11DeviceContext* context, const CommonStates& commonStates) const
{
	//	�ݒ肳��Ă���X�e�[�g���N���A
	context->ClearState();

	//	�����_�[�^�[�Q�b�g�r���[�̃N���A
	context->ClearRenderTargetView(m_renderTexture->GetRenderTargetView(), DirectX::Colors::White);
	//	�[�x�X�e���V���r���[�̃N���A
	context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	//	�`��X�e�[�g��ݒ�
	context->RSSetState(commonStates.CullNone());
	//	�[�x�o�b�t�@���g�p����p�ɐݒ�
	context->OMSetDepthStencilState(commonStates.DepthDefault(), 0xFFFFFFFF);

	//	�`��Ώۂ�ύX
	ID3D11RenderTargetView* rtv = { m_renderTexture->GetRenderTargetView() };
	context->OMSetRenderTargets(1, &rtv, m_depthStencilView.Get());

	//	�r���[�|�[�g��ύX
	context->RSSetViewports(1, &m_viewPort);

}

//--------------------------------------------------------------------------------
// �V���h�E�}�b�v�ɕ`��
//--------------------------------------------------------------------------------
void ShadowMap::RenderShadowMap(ID3D11DeviceContext* context, const SimpleMath::Matrix& world) const
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
void ShadowMap::End(ID3D11DeviceContext* context) const
{
	//	�`��Ώۂ�߂�
	context->OMSetRenderTargets(1, &m_screenRTV, m_screenDSV);

	//	�r���[�|�[�g��߂�
	context->RSSetViewports(1, &m_screenViewPort);

	//	�V�F�[�_�[�̉���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
