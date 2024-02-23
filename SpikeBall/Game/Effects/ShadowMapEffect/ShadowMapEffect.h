/**********************************************
 *
 *  ShadowMapEffect.h
 *  ShadowMapEffect.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#pragma once

class ShadowMapEffect : public DirectX::IEffect, public DirectX::IEffectMatrices
{
private:
	//	�萔�o�b�t�@
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWVP;	// ���[���h�r���[�v���W�F�N�V�����s��
	};

private:
	//	�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_cBuffer;

	//	���_�V�F�[�_�[�̃f�[�^
	std::vector<uint8_t> m_VSData;

	//	���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

private:
	//	���[���h�s��
	DirectX::SimpleMath::Matrix		m_world;
	//	�r���[�s��
	DirectX::SimpleMath::Matrix		m_view;
	//	�ˉe�s��
	DirectX::SimpleMath::Matrix		m_proj;

public:
	ShadowMapEffect(ID3D11Device* device);
	~ShadowMapEffect();


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

};

