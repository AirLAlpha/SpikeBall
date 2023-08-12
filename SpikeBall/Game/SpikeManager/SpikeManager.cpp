/**********************************************
 *
 *  SpikeManager.cpp
 *  �X�p�C�N�̊Ǘ����s��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "SpikeManager.h"
#include "Floor/Floor.h"
#include "Ball/Ball.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

//	�X�p�C�N�̊J�nY���W
const float SpikeManager::SPIKE_START_POS_Y = 15.0f;
//	�X�p�C�N���폜����鍂��
const float SpikeManager::SPIKE_DELETE_POS_Y = -30.0f;
//	�X�p�C�N�̐����Ԋu
const float SpikeManager::SPIKE_GENERATE_INTERVAL = 0.35f;



//	�R���X�g���N�^
SpikeManager::SpikeManager(ID3D11DeviceContext* context):
	m_generateInterval(0.0f),
	m_isHitBall(false)
{
	//	���X�g�̏�����
	for (int i = 0; i < SPIKE_COUNT; i++)
	{
		std::unique_ptr<Spike> newSpike = std::make_unique<Spike>(context);
		m_spikeList.emplace_back(std::move(newSpike));
	}
}

//	�f�X�g���N�^
SpikeManager::~SpikeManager()
{
}


//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void SpikeManager::Update(const float& deltaTime)
{
	GenerateUpdate(deltaTime);

	//	�X�p�C�N�̍X�V����
	for (const auto& spike : m_spikeList)
	{
		if (!spike->GetIsEnable())
			continue;

		spike->Update(deltaTime);

		//	�폜���鍂���ɓ��B�������A�N�e�B�u�ɐݒ肷��
		if (spike->GetPosition().y < SPIKE_DELETE_POS_Y)
			spike->SetIsEnable(false);
	}
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void SpikeManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	for (const auto& spike : m_spikeList)
	{
		if (!spike->GetIsEnable())
			continue;

		spike->Render(view, proj);
	}
}

//--------------------------------------------------------------------------------
// ���Ƃ̏Փ˔���
//--------------------------------------------------------------------------------
void SpikeManager::CheckHitFloor(const Floor& floor)
{
	for (const auto& spike : m_spikeList)
	{
		//	�A�N�e�B�u�łȂ��X�p�C�N�̓X�L�b�v
		if (!spike->GetIsEnable())
			continue;

		//	�Փ˂��Ă�����X�p�C�N������
		if (floor.GetCollider()->CheckHit(*spike->GetCollider()))
		{
			spike->SetIsEnable(false);
		}
	}
}

//--------------------------------------------------------------------------------
// �{�[���Ƃ̏Փ˔���
//--------------------------------------------------------------------------------
void SpikeManager::CheckHitBall(const Ball& ball)
{
	for (const auto& spike : m_spikeList)
	{
		//	�A�N�e�B�u�łȂ��X�p�C�N�̓X�L�b�v
		if (!spike->GetIsEnable())
			continue;

		//	�Փ˂��Ă�����t���O�𗧂Ă�
		if (ball.GetCollider()->CheckHit(*spike->GetCollider()))
		{
			m_isHitBall = true;
		}
	}
}


//--------------------------------------------------------------------------------
// �X�p�C�N�̃����_������
//--------------------------------------------------------------------------------
void SpikeManager::GenerateSpike(const DirectX::SimpleMath::Vector3& range)
{
	//	���g�p�̃X�p�C�N������
	int index = -1;
	for (int i = 0; i < m_spikeList.size(); i++)
	{
		if (!m_spikeList[i]->GetIsEnable())
		{
			index = i;
			break;
		}
	}

	//	������Ȃ������珈�������Ȃ�
	if (index == -1)
		return;

	//	�傫��1�̃����_���ȃx�N�g�����擾
	SimpleMath::Vector3 randPos = GetRandomVec();
	//	�傫����ύX
	randPos.x *= range.x;
	randPos.y = SPIKE_START_POS_Y;
	randPos.z *= range.z;

	m_spikeList[index]->Init(randPos);
}

//--------------------------------------------------------------------------------
// �X�p�C�N�̐����X�V����
//--------------------------------------------------------------------------------
void SpikeManager::GenerateUpdate(const float& deltaTime)
{
	m_generateInterval += deltaTime;
	if (m_generateInterval >= SPIKE_GENERATE_INTERVAL)
	{
		GenerateSpike(SimpleMath::Vector3::One * 9.0f);
		m_generateInterval = 0.0f;
	}
}

