/**********************************************
 *
 *  GameMain.cpp
 *  �Q�[���̃��C���ƂȂ镔�����L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "GameMain.h"

#include "InputManager/InputManager.h"

//	�e�V�[���̃w�b�_�t�@�C��
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/TitleScene/TitleScene.h"

//	�V�[��
const Scene::Scenes START_SCENE = Scene::Scenes::TITLE;		//	�J�n���̃V�[��


GameMain::GameMain():
	m_currentScene(nullptr)
{
}

GameMain::~GameMain()
{
	//	�V�[�������݂���Ƃ��͏I���������s���J������
	if (m_currentScene != nullptr)
	{
		m_currentScene->Finalize();
		delete m_currentScene;
	}
}

//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
void GameMain::Initialize()
{
	ChangeScene(START_SCENE);					//	�V�[���̏�����

	CreateDeviceDependentResources();			//	�f�o�C�X�Ɉˑ����鏉����
	CreateWindowSizeDependentResources();		//	�E�B���h�E�T�C�Y�Ɉˑ����鏉����

	//	�����̏�����
	srand((int)time(0));
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void GameMain::Update(const float& deltaTime)
{
	//	�V�[�������݂��Ȃ��Ƃ��͏������Ȃ�
	if (m_currentScene == nullptr) 
		return;

	//	���݂̃V�[���̍X�V����
	m_currentScene->Update(deltaTime);

	//	�V�[���̑J�ڊm�F
	bool changeSceneFlag = m_currentScene->GetChangeSceneFlag();
	//	�J�ڐ悪�ݒ肳�ꂽ��V�[���̑J�ڂ����s����
	if (changeSceneFlag)
	{
		Scene::Scenes nextScene = m_currentScene->GetNextScene();
		ChangeScene(nextScene);
	}

	//	�I���t���O�̎擾
	bool exitFlag = m_currentScene->GetGameExitFlag();
	//	�I���t���O���L���ɂȂ�����Q�[�����I������
	if (exitFlag ||
		InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void GameMain::Render()
{
	//	�V�[�������݂��Ȃ��Ƃ��͏������Ȃ�
	if (m_currentScene == nullptr) 
		return;

	//	���݂̃V�[���̕`�揈��
	m_currentScene->Render();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void GameMain::Finalize()
{

}


//--------------------------------------------------------------------------------
// �V�[���̐؂�ւ�����
//--------------------------------------------------------------------------------
void GameMain::ChangeScene(Scene::Scenes nextScene)
{
	//	�J�ڐ�̃V�[�����w�肳��Ă��邱�Ƃ�O��Ƃ���
	assert(nextScene != Scene::Scenes::NONE);

	//	�V�[�������݂���Ƃ��͏I���������s���J������
	if (m_currentScene != nullptr)
	{
		m_currentScene->Finalize();
		delete m_currentScene;
	}

	//	�J�ڐ�̃V�[�����쐬����
	switch (nextScene)
	{
	//	�^�C�g���V�[��
	case Scene::Scenes::TITLE:
		m_currentScene = new TitleScene;
		break;

	//	�v���C�V�[��
	case Scene::Scenes::PLAY:
		m_currentScene = new PlayScene;
		break;

	default:
		return;
	}

	//	�V�[�������݂��Ȃ��Ƃ��̓G���[
	assert(m_currentScene != nullptr);

	//	�J�ڐ�̃V�[���̏���������
	m_currentScene->Initialize();
	m_currentScene->CreateDeviceDependentResources();
	m_currentScene->CreateWindowSizeDependentResources();
}

//--------------------------------------------------------------------------------
// �f�o�C�X�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void GameMain::CreateDeviceDependentResources()
{
	//	���݂̃V�[���̏�����
	if (m_currentScene != nullptr)
		m_currentScene->CreateDeviceDependentResources();
}

//--------------------------------------------------------------------------------
// �E�B���h�E�T�C�Y�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void GameMain::CreateWindowSizeDependentResources()
{
	//	���݂̃V�[���̏�����
	if (m_currentScene != nullptr)
		m_currentScene->CreateWindowSizeDependentResources();
}
