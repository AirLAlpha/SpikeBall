/**********************************************
 *
 *  DirectionalLight.cpp
 *  �w�������C�g�i���z���j
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "DirectionalLight.h"

using namespace DirectX;

//	�萔�̏�����
const float DirectionalLight::DEFAULT_DISTANCE = 20.0f;
const float DirectionalLight::DEFAULT_SIZE = 20.0f;
const float DirectionalLight::DEFAULT_NEAR = 1.0f;
const float DirectionalLight::DEFAULT_FAR = 100.0f;

//	�R���X�g���N�^
DirectionalLight::DirectionalLight(const SimpleMath::Vector2& dir):
	m_dist(DEFAULT_DISTANCE),
	m_size(DEFAULT_SIZE)
{
	//	��]��ݒ�
	SetAngle(dir);
}

//	�f�X�g���N�^
DirectionalLight::~DirectionalLight()
{
}


//--------------------------------------------------------------------------------
// ���C�g�̕������擾
//--------------------------------------------------------------------------------
SimpleMath::Vector3 DirectionalLight::GetDirection() const
{
	return SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Down, m_rot);
}

//--------------------------------------------------------------------------------
// ���C�g�̃r���[�s����擾
//--------------------------------------------------------------------------------
SimpleMath::Matrix DirectionalLight::GetLightView() const
{
	SimpleMath::Vector3 target = SimpleMath::Vector3::Zero;		//	�����_
	SimpleMath::Vector3 eye = GetDirection() * m_dist;			//	���C�g�̍��W

	return SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);
}

//--------------------------------------------------------------------------------
// ���C�g�̎ˉe�s����擾
//--------------------------------------------------------------------------------
SimpleMath::Matrix DirectionalLight::GetLightProjection() const
{
	return SimpleMath::Matrix::CreateOrthographic(m_size, m_size, DEFAULT_NEAR, DEFAULT_FAR);
}

//--------------------------------------------------------------------------------
// �����̐ݒ�
//--------------------------------------------------------------------------------
void DirectionalLight::SetAngle(const SimpleMath::Vector2& angle)
{
	m_angle = angle;

	//	��]��ݒ�
	m_rot =
		SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Right, m_angle.y) *
		SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, m_angle.x);
}


#if _DEBUG
#include <string>
#include "InputManager/InputManager.h"

//--------------------------------------------------------------------------------
// �f�o�b�O�p����������
//--------------------------------------------------------------------------------
void DirectionalLight::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	�f�o�b�O�p�t�H���g�̍쐬
	m_debugFont = std::make_unique<Imase::DebugFont>(device, context, L"Resources/Fonts/SegoeUI_18.spritefont");
}
//--------------------------------------------------------------------------------
// �f�o�b�O�p�ړ�����
//--------------------------------------------------------------------------------
void DirectionalLight::DebugMoveUpdate(const float& deltaTime)
{
	//	�L�[���͂ŉ�]������
	InputManager* input = InputManager::GetInstance();
	SimpleMath::Vector2 inputVec;
	if (input->GetKey(InputManager::Keys::W))
		inputVec.y += 1.0f;
	if (input->GetKey(InputManager::Keys::S))
		inputVec.y -= 1.0f;
	if (input->GetKey(InputManager::Keys::A))
		inputVec.x += 1.0f;
	if (input->GetKey(InputManager::Keys::D))
		inputVec.x -= 1.0f;
	m_angle += inputVec * deltaTime;
	SetAngle(m_angle);

	if (input->GetKey(InputManager::Keys::Q))
		m_size -= 0.1f;
	if (input->GetKey(InputManager::Keys::E))
		m_size += 0.1f;

}
//--------------------------------------------------------------------------------
// �f�o�b�O�p�`�揈��
//--------------------------------------------------------------------------------
void DirectionalLight::DebugRender(DirectX::CommonStates* commonStates)
{
	//	������ string �ɕϊ�
	std::wstring dirStr =
		L"x : " + std::to_wstring(m_angle.x) +
		L"  y : " + std::to_wstring(m_angle.y);

	//	������\��
	m_debugFont->AddString(dirStr.c_str(), { 5,5 }, Colors::Black);

	m_debugFont->Render(commonStates);
}
#endif
