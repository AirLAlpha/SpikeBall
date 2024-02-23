/**********************************************
 *
 *  ShadowReciveEffect.h
 *  ShadowReciveEffect.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/007/04
 *
 **********************************************/
#pragma once

//	�O���錾
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
	//	�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;

	//	���_�V�F�[�_�[�f�[�^�[
	std::vector<uint8_t> m_VSData;

	//	���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

private:
	//	�I�u�W�F�N�g�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//	�J�����̃r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//	�J�����̎ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
	//	���C�g�̃r���[�s��
	DirectX::SimpleMath::Matrix m_lightView;
	//	���C�g�̎ˉe�s��
	DirectX::SimpleMath::Matrix m_lightProj;

	//	���C�g�̕���
	DirectX::SimpleMath::Vector3 m_lightDir;
	//	���C�g�̐F
	DirectX::SimpleMath::Vector4 m_lightColor;

	//	�x�[�X�e�N�X�`��
	ID3D11ShaderResourceView* m_baseTexture;
	//	�V���h�E�}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_shadowTexture;


public:
	ShadowReceiveEffect(ID3D11Device* device);
	~ShadowReceiveEffect();

	//	IEffect
	//	�V�F�[�_�[�̓K��
	void Apply(ID3D11DeviceContext* context) override;
	//	���_�V�F�[�_�[�̎擾
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
	//	���C�g�̍s���ݒ�
	void SetLightMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_lightView = view; m_lightProj = proj; }
	//	���C�g�̕�����ݒ�
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_lightDir = dir; }
	//	�x�[�X�e�N�X�`���̐ݒ�
	void SetBaseTexture(ID3D11ShaderResourceView* texture) { m_baseTexture = texture; }
	//	�V���h�E�}�b�v�̃e�N�X�`����ݒ�
	void SetShadowMapTexture(ID3D11ShaderResourceView* texture) { m_shadowTexture = texture; }
	//	���C�g�̐F��ݒ�
	void SetLightColor(const DirectX::SimpleMath::Vector4& color) { m_lightColor = color; }
};
