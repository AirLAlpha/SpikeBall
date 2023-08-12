/**********************************************
 *
 *  Spike.cpp
 *  ���̏������L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/13
 *
 **********************************************/
#include "pch.h"
#include "Spike.h"


using namespace DirectX;

//	���a
const float					Spike::RADIUS = 1.5f;
//	����
const float					Spike::HEIGHT = 2.0f;
//	��]
const SimpleMath::Vector3	Spike::ANGLE = SimpleMath::Vector3(XM_PI, 0.0f, 0.0f);
//	�d��
const float					Spike::GRAVITY = 0.05f;
//	�R���C�_�[�̃I�t�Z�b�g
const SimpleMath::Vector3	Spike::COLLIDER_OFFSET = SimpleMath::Vector3::Up * 0.35f;


//	�R���X�g���N�^
Spike::Spike(ID3D11DeviceContext* context):
	m_isEnable(false)
{
	//	�W�I���g���b�N�v���~�e�B�u�̍쐬
	m_geometricPrimitive = GeometricPrimitive::CreateCone(context, RADIUS, HEIGHT);
	//	�R���C�_�[�̍쐬
	m_collider = std::make_unique<SphereCollider>(COLLIDER_OFFSET, RADIUS / 3.0f);
}

//	�f�X�g���N�^
Spike::~Spike()
{
}


//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
void Spike::Init(const DirectX::SimpleMath::Vector3& pos)
{
	m_pos = pos;
	m_acce = SimpleMath::Vector3::Zero;
	m_collider->SetPosition(m_pos);

	m_isEnable = true;
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Spike::Update(const float& deltaTime)
{
	//	�d�͂����Z
	m_acce += SimpleMath::Vector3::Down * GRAVITY;

	//	�����x�����W�ɉ��Z
	m_pos += m_acce * deltaTime;
	m_collider->SetPosition(m_pos + COLLIDER_OFFSET);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Spike::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//	���[���h�s��̍쐬
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateFromYawPitchRoll(ANGLE) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	�`��
	m_geometricPrimitive->Draw(world, view, proj);
}
