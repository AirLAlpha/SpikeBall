/**********************************************
 *
 *  Scene.h
 *  �e�V�[���̊���ƂȂ�N���X���L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/25
 *
 **********************************************/
#pragma once
#include <SimpleMath.h>

#include "UserResources.h"

class Scene
{
public:
	enum class Scenes
	{
		NONE = 0,	//	�J�ڂȂ�

		TITLE,		//	�^�C�g��
		PLAY,		//	�v���C

		OVER_ID		//	�͈͊O
	};

private:
	Scenes	m_nowScene;			//	���݂̃V�[��
	Scenes	m_nextScene;		//	�J�ڐ�̃V�[��
	bool	m_changeSceneFlag;	//	�V�[���J�ڃt���O
	bool	m_gameExitFlag;		//	�Q�[���̏I���t���O�̎擾

public:
	//	�R���X�g���N�^
	Scene(Scenes nowScene) :
		m_nowScene(nowScene),
		m_nextScene(Scenes::NONE),
		m_changeSceneFlag(false),
		m_gameExitFlag(false)
	{
	};

	virtual ~Scene() {};				//	�f�X�g���N�^

	virtual void Initialize()					= 0;	//	����������
	virtual void Update(const float& deltaTime)	= 0;	//	�X�V����
	virtual void Render()						= 0;	//	�`�揈��
	virtual void Finalize()						= 0;	//	�I������

	virtual void CreateDeviceDependentResources()		= 0;	//	�f�o�C�X�Ɉˑ����鏉��������
	virtual void CreateWindowSizeDependentResources()	= 0;	//	�E�B���h�E�T�C�Y�Ɉˑ����鏉����

	void ChangeScene()	{ m_changeSceneFlag = true; }			//	�V�[���J�ڂ̎��s�i�t���O��L�����j
	void GameExit()		{ m_gameExitFlag = true; }				//	�Q�[���̏I���i�t���O��L�����j

	void SetNextScene(Scenes nextScene) { m_nextScene = nextScene; }	//	�J�ڐ�̐ݒ�

	Scenes	GetNextScene()			{ return m_nextScene; }				//	�J�ڐ�̎擾
	Scenes* GetNextScenePtr()		{ return &m_nextScene; }			//	�J�ڐ�������|�C���^���擾
	bool	GetChangeSceneFlag()	{ return m_changeSceneFlag; }		//	�V�[���J�ڃt���O�̎擾
	bool*	GetChangeSceneFlagPtr()	{ return &m_changeSceneFlag; }		//	�V�[���J�ڃt���O�������|�C���^�̎擾
	bool	GetGameExitFlag()		{ return m_gameExitFlag; }			//	�Q�[���̏I���t���O�̎擾
};
