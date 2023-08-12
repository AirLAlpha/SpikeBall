/**********************************************
 *
 *  Timer.cpp
 *  �^�C�}�[UI
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "Timer.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	���Ԃ̍ő�l
const float Timer::TIME_MAX = (60.0f * 59 + 59 + 0.999f);
//	�A�E�g���C���̕�
const int	Timer::OUTLINE_WIDTH = 3;


//	�R���X�g���N�^
Timer::Timer(ID3D11Device* device, ID3D11DeviceContext* context, const SimpleMath::Vector2& pos):
	SpriteNumber(device, context),
	m_pos(pos),
	m_time(0.0f),
	m_isStop(false)
{
}

//	�f�X�g���N�^
Timer::~Timer()
{
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void Timer::Update(const float& deltaTime)
{
	//	��~���͏������Ȃ�
	if (m_isStop)
		return;

	//	���Ԃ����Z����
	m_time += deltaTime;
	//	���l���I�[�o�[�t���[���Ȃ��悤�ɃN�����v����
	m_time = Clump(m_time, 0.0f, TIME_MAX);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void Timer::Render(const DirectX::CommonStates& states)
{
	SimpleMath::Vector2 textureSize = GetTextureSize();
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(textureSize.x / TEXTURE_SPLIT_COUNT.x, textureSize.y / TEXTURE_SPLIT_COUNT.y);

	int sec = static_cast<int>(m_time) % 60;					//	�b
	int min = static_cast<int>(m_time / 60);					//	��
	int ms	= static_cast<int>((m_time - (int)m_time) * 100);	//	�~���b

	RECT colonRect;							//	�R�����p��RECT���쐬
	colonRect.left		= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * 10);
	colonRect.right		= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * 11);
	colonRect.top		= 0;
	colonRect.bottom	= static_cast<LONG>(TEXTURE_SPRITE_SIZE.y);

	auto offset = [](int dig) { return NUMBER_GAP * static_cast<float>(dig); };		//	�I�t�Z�b�g�̌v�Z�����郉���_��

	Begin(states);		//	�`��J�n

	//	���̕`��
	DrawNumber(min, m_pos, Anchor::LEFT, SimpleMath::Vector2::Zero, OUTLINE_WIDTH, 2);
	//	�R�����̕`��
	SimpleMath::Vector2 colonOffset = offset(2);
	DrawNumber(colonRect, m_pos, Anchor::LEFT, colonOffset, OUTLINE_WIDTH);
	//	�b�̕`��
	SimpleMath::Vector2 secOffset = offset(3);
	DrawNumber(sec, m_pos, Anchor::LEFT, secOffset, OUTLINE_WIDTH, 2);
	//	�R�����̕`��
	colonOffset = offset(5);
	DrawNumber(colonRect, m_pos, Anchor::LEFT, colonOffset, OUTLINE_WIDTH);
	//	�~���b�̕`��
	SimpleMath::Vector2 msOffset = offset(6);
	DrawNumber(ms, m_pos, Anchor::LEFT, msOffset, OUTLINE_WIDTH, 2);

	End();			//	�`��I��
}

//--------------------------------------------------------------------------------
// ���Z�b�g
//--------------------------------------------------------------------------------
void Timer::Reset()
{
	//	���Ԃ����Z�b�g
	m_time = 0.0f;
}
