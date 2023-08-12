/**********************************************
 *
 *  ShadowMap.h
 *  ShadowMap.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#pragma once

#include "Effects/ShadowMapEffect/ShadowMapEffect.h"

//	�O���錾
class DirectionalLight;


class ShadowMap
{
private:
	//	�V���h�E�}�b�v�p�G�t�F�N�g
	std::unique_ptr<ShadowMapEffect>					m_shadowMapEffect;
	//	�V���h�E�}�b�v�p�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			m_inputLayout;

	//	�[�x�o�b�t�@�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_depthStencilTex;
	//	�[�x�r���[�̍쐬
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		m_depthStencilView;

	//	Z�l�p�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				m_zTexture;
	//	Z�l�p�����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		m_zRTV;
	//	Z�l�p�V�F�[�_�[���\�[�X�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_zSRV;

	D3D11_VIEWPORT			m_viewPort;	//	�V���h�E�}�b�v�̃r���[�|�[�g

private:
	ID3D11RenderTargetView* m_screenRTV;			//	�X�N���[���̃����_�[�^�[�Q�b�g�r���[
	ID3D11DepthStencilView* m_screenDSV;			//	�X�N���[���̐[�x�X�e���V���r���[
	D3D11_VIEWPORT			m_screenViewPort;		//	�X�N���[���̃r���[�|�[�g

	DirectionalLight*		m_light;				//	�`��Ɏg�p���郉�C�g

public:
	ShadowMap(DX::DeviceResources* deviceResources, UINT resolution);
	~ShadowMap();

	//	�V���h�E�}�b�v�ɕ`��J�n
	void Begin(ID3D11DeviceContext* context, DirectX::CommonStates* commonStates);
	//	�V���h�E�}�b�v�ɕ`��
	void RenderShadowMap(ID3D11DeviceContext* context, const DirectX::SimpleMath::Matrix& world);
	//	�V���h�E�}�b�v�ɕ`��I��
	void End(ID3D11DeviceContext* context);


public:
	//	�V���h�E�}�b�v�e�N�X�`���̎擾
	ID3D11ShaderResourceView* GetShadowMapTexture() { return m_zSRV.Get(); }
	ID3D11Texture2D* GetShadowMapRawTexture()		{ return m_zTexture.Get(); }

public:
	//	���C�g�̐ݒ�
	void SetLight(DirectionalLight* light) { m_light = light; }

};
