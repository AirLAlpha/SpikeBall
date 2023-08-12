/**********************************************
 *
 *  PlayScene.h
 *  PlayScene.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/25
 *
 **********************************************/
#pragma once
#include "../Scene.h"

#include "Camera/DebugCamera/DebugCamera.h"
#include "Camera/Camera.h"
#include "Floor/Floor.h"
#include "Ball/Ball.h"
#include "SpikeManager/SpikeManager.h"
#include "ResultCanvas/ResultCanvas.h"
#include "Timer/Timer.h"
#include "Background/Background.h"

class PlayScene : public Scene
{
private:
	//	���S���鍂��
	static const float DEAD_HEIGHT;

private:
	//	�f�o�b�O�J����
	std::unique_ptr<DebugCamera>	m_debugCamera;
	//	�J����
	std::unique_ptr<Camera>			m_camera;

	//	��
	std::unique_ptr<Floor>			m_floor;
	//	�{�[��
	std::unique_ptr<Ball>			m_ball;

	//	�X�p�C�N�}�l�[�W���[
	std::unique_ptr<SpikeManager>	m_spikeManager;

	//	���U���g
	std::unique_ptr<ResultCanvas>	m_resultCanvas;
	//	�^�C�}�[
	std::unique_ptr<Timer>			m_timer;
	//	�^�C�}�[���W
	DirectX::SimpleMath::Vector2	m_timerPos;

	//	�w�i
	std::unique_ptr<Background>		m_background;

private:
	//	�Q�[���I�[�o�[
	bool	m_isGameOver;


private:
	//	�J�����̍X�V
	void CameraUpdate(const float& deltaTime);
	//	�Q�[���I�[�o�[���̍X�V����
	void GameOverUpdate(const float& deltaTime);

public:
	PlayScene();						//	�R���X�g���N�^
	~PlayScene();						//	�f�X�g���N�^

	void Initialize()						override;		//	����������
	void Update(const float& deltaTime)		override;		//	�X�V����
	void Render()							override;		//	�`�揈��
	void Finalize()							override;		//	�I������

	void CreateDeviceDependentResources() override;			//	�f�o�C�X�Ɉˑ����鏉��������
	void CreateWindowSizeDependentResources() override;		//	�E�B���h�E�T�C�Y�Ɉˑ����鏉��������
};