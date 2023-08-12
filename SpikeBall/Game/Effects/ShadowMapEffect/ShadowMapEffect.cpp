/**********************************************
 *
 *  ShadowMapEffect.cpp
 *  �V���h�E�}�b�v�p�G�t�F�N�g
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "ShadowMapEffect.h"
#include "Utilities/ReadData.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

//	�R���X�g���N�^
ShadowMapEffect::ShadowMapEffect(ID3D11Device* device)
{
	//	�V�F�[�_�[�̓ǂݍ���
	m_VSData = DX::ReadData(L"Resources/Shaders/ShadowMap_VS.cso");
	std::vector<uint8_t> PSData = DX::ReadData(L"Resources/Shaders/ShadowMap_PS.cso");

	//	���_�V�F�[�_�[�̍쐬
	DX::ThrowIfFailed(
		device->CreateVertexShader(m_VSData.data(), m_VSData.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);
	//	�s�N�Z���V�F�[�_�[�̍쐬
	DX::ThrowIfFailed(
		device->CreatePixelShader(PSData.data(), PSData.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	�萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = GetConstantBufferSize<ConstBuffer>();
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = FALSE;
	device->CreateBuffer(&desc, nullptr, m_cBuffer.ReleaseAndGetAddressOf());
}

//	�f�X�g���N�^
ShadowMapEffect::~ShadowMapEffect()
{
}


//--------------------------------------------------------------------------------
// �V�F�[�_�[�̓K��
//--------------------------------------------------------------------------------
void ShadowMapEffect::Apply(ID3D11DeviceContext* context)
{
	//	�萔�o�b�t�@�̍X�V
	ConstBuffer buff = {};
	SimpleMath::Matrix WVP = m_world * m_view * m_proj;
	buff.matWVP = WVP.Transpose();
	context->UpdateSubresource(m_cBuffer.Get(), 0, NULL, &buff, 0, 0);

	//	�萔�o�b�t�@��ݒ�
	context->VSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_cBuffer.GetAddressOf());

	//	�V�F�[�_�[��ݒ�
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
}

//--------------------------------------------------------------------------------
// ���_�V�F�[�_�[�̎擾
//--------------------------------------------------------------------------------
void ShadowMapEffect::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	//	�ǂ��炩�� nullptr �Ȃ�G���[
	assert(pShaderByteCode != nullptr && pByteCodeLength != nullptr);

	//	����n��
	*pShaderByteCode = m_VSData.data();
	*pByteCodeLength = m_VSData.size();
}