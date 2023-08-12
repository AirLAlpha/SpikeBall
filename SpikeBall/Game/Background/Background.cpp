/**********************************************
 *
 *  Background.cpp
 *  �w�i
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "Background.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	�T�C�Y
const float Background::SIZE = 100.0f;
//	�X�s�[�h
const float Background::SPEED= 0.1f;


//	�R���X�g���N�^
Background::Background(ID3D11Device* device, ID3D11DeviceContext* context):
	m_angle(0.0f)
{
	//	�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);

	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect< VertexPositionNormalTexture>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	�W�I���g���b�N�v���~�e�B�u�̍쐬
	m_geometricPrimitive = GeometricPrimitive::CreateSphere(context, SIZE);

}

//	�f�X�g���N�^
Background::~Background()
{
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Background::Update(const float& deltaTime)
{
	m_angle += deltaTime * SPEED;
	m_angle = Repeat(m_angle, XM_2PI);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Background::Render(
	ID3D11DeviceContext* context, 
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj
)
{
	//	���[���h�s��̍쐬
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateRotationY(m_angle);

	//	�s��̐ݒ�
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	//	�`��
	m_geometricPrimitive->Draw(m_basicEffect.get(), m_inputLayout.Get(), false, true);
}
