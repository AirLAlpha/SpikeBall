/**********************************************
 *
 *  Ball.cpp
 *  �{�[��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/27
 *
 **********************************************/
#include "pch.h"
#include "Ball.h"
#include "UserResources.h"
#include "Floor/Floor.h"

using namespace DirectX;

//	�d��
const float Ball::GRAVITY = 0.3f;
//	���x�����p
const float Ball::SPEED_MOD = 0.1f;



//	�R���X�g���N�^
Ball::Ball(ID3D11Device* device, const SimpleMath::Vector3& pos, const float radius) :
	m_pos(pos + SimpleMath::Vector3::Right * 0.0001f),
	m_radius(radius),
	m_onGround(false)
{
	//	�G�t�F�N�g�t�@�N�g���[�̍쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	//	���f���̍쐬
	m_model = Model::CreateFromCMO(device, L"Resources/Models/ball.cmo", *fx);

	//	�R���C�_�[�̏�����
	m_collider = std::make_unique<SphereCollider>(pos, radius);
}

//	�f�X�g���N�^
Ball::~Ball()
{
}


//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Ball::Update(const float& deltaTime)
{
	//	�ړ�����
	MoveUpdate(deltaTime);

	//	�R���C�_�[�̍��W���X�V
	m_collider->SetPosition(m_pos);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Ball::Render(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	���[���h�s��̍쐬
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateScale(m_radius) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	�`��
	m_model->Draw(context, states, world, view, proj);
}

//--------------------------------------------------------------------------------
// ���Ƃ̏Փ˔���
//--------------------------------------------------------------------------------
void Ball::CheckHitFloor(const Floor& floor)
{
	SimpleMath::Vector3 hitResult;
	if (floor.GetCollider()->CheckHit(*m_collider, &hitResult))
	{
		//	���̖@�����擾
		m_groundNormal = floor.GetCollider()->GetNormal();
		//	���̊p�x���擾
		m_groundAngle = floor.GetAngle();

		//	�Փ˂����_����@�������ɔ��a�������߂�
		auto newPos = hitResult + m_groundNormal * m_radius;

		//	�V�������W����
		m_pos = newPos;
		m_collider->SetPosition(newPos);

		//	�ڒn�t���O��L����
		m_onGround = true;
		return;
	}

	//	�Փ˂��Ă��Ȃ���΁A�ڒn�t���O�𖳌���
	m_onGround = false;
}


//--------------------------------------------------------------------------------
// �ړ�����
//--------------------------------------------------------------------------------
void Ball::MoveUpdate(const float& deltaTime)
{
	if (m_onGround)
	{
		//	�ڒn���Ă���Ƃ�

		//	�����x�Ƃ̓��ς����
		float groundDot = m_acce.Dot(m_groundNormal);
		//	���̖@�������ɑ΂�������x��ł�����
		m_acce += m_groundNormal * -groundDot;

		//	�n�ʂ̉�]����ڕW�ƂȂ�����x���쐬
		SimpleMath::Vector3 forward = SimpleMath::Vector3::Forward * -m_groundAngle.x;
		SimpleMath::Vector3 right = SimpleMath::Vector3::Right * -m_groundAngle.y;

		//	�ڕW�ƂȂ�����x�𐳋K�����A�X�s�[�h����Z
		SimpleMath::Vector3 targetAcce = (forward + right);
		targetAcce.Normalize();
		targetAcce *= SPEED_MOD;

		//	�����x�����Z
		m_acce += targetAcce;
	}
	else
	{
		m_acce.y -= GRAVITY;
	}

	//	�����x�����W�ɉ��Z
	m_pos += m_acce * deltaTime;

	//	�ړ��ʂ̌v�Z
	float acceLength = m_acce.Length() * deltaTime * 1.5f;
	if (acceLength <= FLT_EPSILON)
	{
		m_rot = SimpleMath::Quaternion::Identity;
		return;
	}

	//	�����x�����]�����쐬
	SimpleMath::Vector3 axis = SimpleMath::Vector3(m_acce.z, m_acce.y, -m_acce.x);
	//	��]�̍쐬
	SimpleMath::Quaternion deltaRot = SimpleMath::Quaternion::CreateFromAxisAngle(axis, acceLength);

	m_rot *= deltaRot;
}