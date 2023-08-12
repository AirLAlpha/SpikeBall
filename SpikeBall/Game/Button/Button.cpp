/**********************************************
 *
 *  Button.cpp
 *  UI�̃{�^���Ɋւ��鏈�����L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "Button.h"

#include <WICTextureLoader.h>
#include "DeviceResources.h"
#include "InputManager/InputManager.h"

using namespace DirectX;


//	�R���X�g���N�^
Button::Button(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	const SimpleMath::Vector2& pos,
	const SimpleMath::Vector2& size, 
	std::function<void(void)> func, 
	const wchar_t * texturePath
):
	m_pos(pos),
	m_size (size),
	m_pressedFunction (func),
	m_color (Colors::White)
{
	//	�V�F�C�v�����_�[�̍쐬
	m_shapeRender = std::make_unique<ShapeRender2D>(device, context);

	//	�e�N�X�`���̗L�����t���O
	bool enableTexture = texturePath != nullptr;
	if (!enableTexture)
		return;

	//	�e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			texturePath,
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);
	//	�V�F�C�v�����_�[�ɐݒ�
	m_shapeRender->SetTextureEnable(device, true);
	m_shapeRender->SetTexture(m_texture.Get());
}

//	�f�X�g���N�^
Button::~Button()
{
}


//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Button::Update()
{
	auto mouseState = InputManager::GetInstance()->GetMouse()->GetState();
	auto bst = InputManager::GetInstance()->GetMouseButtonStateTracker();

	SimpleMath::Vector2 pos1 = m_pos - m_size / 2;
	SimpleMath::Vector2 pos2 = m_pos + m_size / 2;
	;
	if (pos1.x < mouseState.x && pos2.x > mouseState.x &&
		pos1.y < mouseState.y && pos2.y > mouseState.y)
	{
		//	�J�[�\�����{�^����ɂ��鎞
		m_color = Colors::Red;

		//	�N���b�N����Ă��鎞�͐F���Â�����
		if (mouseState.leftButton)
			m_color = Colors::DarkRed;

		//	�N���b�N�𗣂������ɏ��������s
		if (m_pressedFunction && bst->leftButton == Mouse::ButtonStateTracker::RELEASED )
		{
			m_pressedFunction();
		}

	}
	else
	{
		m_color = Colors::White;
	}
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Button::Render(ID3D11DeviceContext* context, const CommonStates& states)
{
	m_shapeRender->Begin(context, states);

	SimpleMath::Vector2 p1 = m_pos - m_size / 2;
	SimpleMath::Vector2 p2 = m_pos + m_size / 2;
	m_shapeRender->DrawBox(p1, p2, m_color, SimpleMath::Vector2(1.0f,1.0f));

	m_shapeRender->End();
}
