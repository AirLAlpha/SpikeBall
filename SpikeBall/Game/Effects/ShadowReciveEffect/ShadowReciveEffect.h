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

	//	�V���h�E�}�b�v�e�N�X�`��
	ID3D11ShaderResourceView* m_shadowTexture;


public:
	ShadowReciveEffect(ID3D11Device* device);
	~ShadowReciveEffect();

	//	IEffect
	//	�V�F�[�_�[�̓K��
	void Apply(ID3D11DeviceContext* context) override;
	//	���_�V�F�[�_�[�̎擾
	void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) override;

public:
	//	�s��̐ݒ�
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	//	�J�����̍s���ݒ�
	void SetCameraMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_view = view; m_proj = proj; }
	//	���C�g�̍s���ݒ�
	void SetLightMatrix(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) { m_lightView = view; m_lightProj = proj; }

	//	���C�g�̕�����ݒ�
	void SetLightDirection(const DirectX::SimpleMath::Vector3& dir) { m_lightDir = dir; }

	//	�V���h�E�}�b�v�̃e�N�X�`����ݒ�
	void SetShadowMapTexture(ID3D11ShaderResourceView* texture) { m_shadowTexture = texture; }
};
