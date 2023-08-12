/**********************************************
 *
 *  Timer.h
 *  Timer.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#pragma once
#include "SpriteNumber/SpriteNumber.h"

class Timer : public SpriteNumber
{
private:
	//	���Ԃ̍ő�l
	static const float	TIME_MAX;
	//	�A�E�g���C���̕�
	static const int	OUTLINE_WIDTH;

private:
	//	�`��ʒu
	DirectX::SimpleMath::Vector2	m_pos;

	//	����
	float m_time;

	//	��~�t���O
	bool m_isStop;

public:
	//	�R���X�g���N�^
	Timer(ID3D11Device* device, ID3D11DeviceContext* context, const DirectX::SimpleMath::Vector2& pos);
	//	�f�X�g���N�^
	~Timer();

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(const DirectX::CommonStates& states);

	//	���Z�b�g
	void Reset();

public:
	//	��~�̐ݒ�
	void SetIsStop(bool stop) { m_isStop = stop; }
	//	���W�̐ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2& pos) { m_pos = pos; }
	
public:
	//	���Ԃ̎擾
	float GetTime() { return m_time; }
};
