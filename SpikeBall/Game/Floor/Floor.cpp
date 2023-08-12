/**********************************************
 *
 *  Floor.cpp
 *  ���Ɋւ��鏈��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/27
 *
 **********************************************/
#include "pch.h"
#include "Floor.h"
#include "UserResources.h"
#include "InputManager/InputManager.h"

using namespace DirectX;

//	���f���̃X�P�[��
const SimpleMath::Vector3	Floor::MODEL_SCALE	= SimpleMath::Vector3::One * 1.5f;
//	���f���̃I�t�Z�b�g
const SimpleMath::Vector3	Floor::MODEL_OFFSET	= SimpleMath::Vector3::Down * 0.375f;
//	�R���C�_�[�̃X�P�[��
const SimpleMath::Vector2	Floor::COLLIDER_SCALE = SimpleMath::Vector2::One * 15.0f;
//	��]���x
const float					Floor::ROTATE_SPEED = 1.2f;


//	�R���X�g���N�^
Floor::Floor(ID3D11Device* device)
{
	//	�G�t�F�N�g�t�@�N�g���[�̍쐬
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);

	//	���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(device, L"Resources/Models/floor.cmo", *fx);

	//	�R���C�_�[�̍쐬
	m_collider = std::make_unique<PlaneCollider>(
		SimpleMath::Vector3::Zero,
		COLLIDER_SCALE,
		m_rot
	);
}

//	�f�X�g���N�^
Floor::~Floor()
{
}


//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Floor::Update(const float& deltaTime)
{
	//	��]����
	RotateUpdate(deltaTime);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Floor::Render(
	ID3D11DeviceContext* context, 
	const DirectX::CommonStates& states,
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj
)
{
	//	���[���h�ϊ��s����v�Z
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateScale(MODEL_SCALE) *
		SimpleMath::Matrix::CreateTranslation(MODEL_OFFSET) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot);

	//	���f���̕`��
	m_model->Draw(context, states, world, view, proj);

}



//--------------------------------------------------------------------------------
// ��]����
//--------------------------------------------------------------------------------
void Floor::RotateUpdate(const float& deltaTime)
{
	//	���̓}�l�[�W���[�̎擾
	auto input = InputManager::GetInstance();

	//	���͂ɉ����ĉ�]������
	SimpleMath::Vector2 deltaAngle;
	if (input->GetKey(InputManager::Keys::W))
		deltaAngle.x += -deltaTime;
	if (input->GetKey(InputManager::Keys::S))
		deltaAngle.x += deltaTime;
	if (input->GetKey(InputManager::Keys::D))
		deltaAngle.y += -deltaTime;
	if (input->GetKey(InputManager::Keys::A))
		deltaAngle.y += deltaTime;

	//	��]�ʂ����Z
	m_angle += deltaAngle * ROTATE_SPEED;

	//	��]�ʂ���N�H�[�^�j�I���̍쐬
	m_rot = SimpleMath::Quaternion::CreateFromYawPitchRoll(SimpleMath::Vector3(m_angle.x, 0.0f, m_angle.y));
	//	�R���C�_�[�ɔ��f
	m_collider->SetRotate(m_rot);
}