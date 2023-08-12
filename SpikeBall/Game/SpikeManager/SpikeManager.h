/**********************************************
 *
 *  SpikeManager.h
 *  SpikeManager.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once
#include "Spike/Spike.h"

//	�O���錾
class Floor;
class Ball;


class SpikeManager
{
private:
	//	�X�p�C�N�̑���
	static const int	SPIKE_COUNT = 30;
	//	�X�p�C�N�̊J�nY���W
	static const float	SPIKE_START_POS_Y;
	//	�X�p�C�N���폜�����Y���W
	static const float	SPIKE_DELETE_POS_Y;
	//	�X�p�C�N�̐����Ԋu
	static const float	SPIKE_GENERATE_INTERVAL;

private:
	//	�X�p�C�N�̔z��
	std::vector<std::unique_ptr<Spike>> m_spikeList;

	//	�����Ԋu
	float			m_generateInterval;

	//	�v���C���[�Ƃ̏Փ˃t���O
	bool			m_isHitBall;

private:
	//	�X�p�C�N�̃����_������
	void GenerateSpike(const DirectX::SimpleMath::Vector3& range);
	//	�X�p�C�N�̐�������
	void GenerateUpdate(const float& deltaTime);

public:
	SpikeManager(ID3D11DeviceContext* context);
	~SpikeManager();

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//	���Ƃ̏Փ˔���
	void CheckHitFloor(const Floor& floor);
	//	�{�[���Ƃ̏Փ˔���
	void CheckHitBall(const Ball& ball);

public:
	//	�v���C���[�Ƃ̏Փ˃t���O���擾
	bool GetIsHitPlayer() { return m_isHitBall; }

};
