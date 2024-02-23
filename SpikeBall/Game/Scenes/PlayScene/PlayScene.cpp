/**********************************************
 *
 *  PlayScene.cpp
 *  �C���Q�[�������̏������L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "PlayScene.h"
#include "InputManager/InputManager.h"
#include "Utilities/UtilityFunction.h"
#include "Utilities/Screen.h"

using namespace DirectX;

//	���S���鍂��
const float PlayScene::DEAD_HEIGHT = -15.0f;


//	�R���X�g���N�^
PlayScene::PlayScene():
	Scene(Scenes::PLAY),
	m_isGameOver(false)
{
	SetNextScene(Scenes::TITLE);
}

//	�f�E�X�g���N�^
PlayScene::~PlayScene()
{
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
void PlayScene::Initialize()
{
	//	�f�o�b�O�J�����̍쐬
	m_debugCamera = std::make_unique<DebugCamera>();
	//	�J�����̍쐬
	m_camera = std::make_unique<Camera>();
	m_camera->SetPosition(SimpleMath::Vector3(0.0f, 15.0f, 10.0f));
	m_camera->SetFov(0.01f);
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void PlayScene::Update(const float& deltaTime)
{
	//	�f�o�b�O�J�����̍X�V
	m_debugCamera->Update();
	CameraUpdate(deltaTime);

	//	�w�i�̍X�V
	m_background->Update(deltaTime);

	if (m_isGameOver)
	{
		GameOverUpdate(deltaTime);
		return;
	}

	//	���̍X�V
	m_floor->Update(deltaTime);
	//	�{�[���̍X�V
	m_ball->Update(deltaTime);
	//	�X�p�C�N�}�l�[�W���[�̍X�V
	m_spikeManager->Update(deltaTime);

	//	���Ƃ̏Փ˔���
	m_ball->CheckHitFloor(*m_floor);

	//	���Ə��Ƃ̏Փ˔���
	m_spikeManager->CheckHitFloor(*m_floor);
	//	���ƃv���C���[�Ƃ̏Փ˔���
	m_spikeManager->CheckHitBall(*m_ball);

	//	�^�C�}�[�̍X�V
	m_timer->Update(deltaTime);

	//	���ȉ��̍����ɍs�� or ���ɓ���������Q�[���I�[�o�[
	if (m_ball->GetPosition().y < DEAD_HEIGHT ||
		m_spikeManager->GetIsHitPlayer())
	{
		m_isGameOver = true;
	}


	if (InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Enter))
		ChangeScene();
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void PlayScene::Render()
{
	//	�K�v�ȃ��\�[�X�̎擾
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	�J�����̍s����擾
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	//	�V���h�E�}�b�v�ւ̕`��
	m_shadowMap->Begin(context, *states);

	m_floor->ShadowMapRender(*m_shadowMap, context, *states);
	m_spikeManager->ShadowMapRender(*m_shadowMap, context);
	m_ball->ShadowMapRender(*m_shadowMap, context, *states);

	m_shadowMap->End(context);


	//	�w�i�̕`��
	m_background->Render(context, view, proj);
	//	���̕`��
	m_floor->Render(context, *states, view, proj);
	//	�{�[���̕`��
	m_ball->Render(context, *states, view, proj);

	//	�X�p�C�N�̕`��
	m_spikeManager->Render(view, proj);

	//	�Q�[���I�[�o�[���̓��U���g��`��
	if (m_isGameOver)
		m_resultCanvas->Render(context, *states);

	//	�^�C�}�[�̕`��
	m_timer->Render(*states);
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void PlayScene::Finalize()
{

}

//--------------------------------------------------------------------------------
// �f�o�C�X�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void PlayScene::CreateDeviceDependentResources()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	���̐���
	m_floor = std::make_unique<Floor>(device);
	//	�{�[���̐���
	m_ball = std::make_unique<Ball>(device, SimpleMath::Vector3::Up * 0.01f, 0.5f);

	//	�X�p�C�N�}�l�[�W���[�̍쐬
	m_spikeManager = std::make_unique<SpikeManager>(context);

	//	�w�i�̐���
	m_background = std::make_unique<Background>(device, context);

	//	�^�C�}�[�̐���
	m_timerPos = Screen::SCREEN_CENTER() + SimpleMath::Vector2(Screen::WIDTH / -4, -430);
	m_timer = std::make_unique<Timer>(device, context, m_timerPos);

	//	���C�g�̍쐬
	m_light = std::make_unique<DirectionalLight>(SimpleMath::Vector2(0.0f, XM_PI));
	m_floor->SetLight(m_light.get());

	//	�V���h�E�}�b�v�̍쐬
	m_shadowMap = std::make_unique<ShadowMap>(*UserResources::GetInstance()->GetDeviceResources(), 512);
	m_shadowMap->SetLight(m_light.get());

	//	���U���g�L�����o�X�̍쐬
	m_resultCanvas = std::make_unique<ResultCanvas>(device, context, GetNextScenePtr(), GetChangeSceneFlagPtr());
}

//--------------------------------------------------------------------------------
// �E�B���h�E�T�C�Y�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void PlayScene::CreateWindowSizeDependentResources()
{
}


//--------------------------------------------------------------------------------
// �J�����̍X�V����
//--------------------------------------------------------------------------------
void PlayScene::CameraUpdate(const float& deltaTime)
{
	//	�Q�[���I�[�o�[����Ȃ��Ƃ�
	if (!m_isGameOver)
	{
		float fov = Lerp(m_camera->GetFov(), 60.0f, deltaTime * 10.0f);
		m_camera->SetFov(fov);
	}
	else
	{
		//	�x�N�g���̌v�Z
		SimpleMath::Vector3 playerPos = m_ball->GetPosition();
		SimpleMath::Vector3 toPlayerVec = playerPos - m_camera->GetPosition();
		toPlayerVec.Normalize();

		//	�^�[�Q�b�g���W
		SimpleMath::Vector3 targetPos = playerPos - toPlayerVec * 5.0f;

		//	���X�Ƀv���C���[�Ɏ��_�����킹��
		SimpleMath::Vector3 lerpedPos = SimpleMath::Vector3::Lerp(m_camera->GetPosition(), targetPos, deltaTime * 10.0f);
		SimpleMath::Vector3 lerpedTarget = SimpleMath::Vector3::Lerp(m_camera->GetTarget(), playerPos, deltaTime * 10.0f);

		//	���W�ƃ^�[�Q�b�g��ݒ�
		m_camera->SetPosition(lerpedPos);
		m_camera->SetTarget(lerpedTarget);
	}
}

//--------------------------------------------------------------------------------
// �Q�[���I�[�o�[���̍X�V����
//--------------------------------------------------------------------------------
void PlayScene::GameOverUpdate(const float& deltaTime)
{
	//	�L�����o�X�̍X�V
	m_resultCanvas->Update(deltaTime);

	//	�^�C�}�[�̍��W���X�V����
	m_timerPos = Screen::SCREEN_CENTER() + SimpleMath::Vector2(Screen::WIDTH / -4, 20) + m_resultCanvas->GetCanvasPos();
	m_timer->SetPosition(m_timerPos);
}
