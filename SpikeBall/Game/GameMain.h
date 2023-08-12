/**********************************************
 *
 *  GameMain.h
 *  GameMain.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/25
 *
 **********************************************/
#pragma once
#include "Scenes/Scene.h"

class GameMain
{
private:
	Scene* m_currentScene;	//	���݂̃V�[��

	

private:
	void ChangeScene(Scene::Scenes nextScene);		//	�V�[���̐؂�ւ�����

public:
	GameMain();			//	�R���X�g���N�^
	~GameMain();		//	�f�X�g���N�^

	void Initialize();							//	����������
	void Update(const float& deltaTime);		//	�X�V����
	void Render();								//	�`�揈��
	void Finalize();							//	�I������

	void CreateDeviceDependentResources();		//	�f�o�C�X�Ɉˑ����鏉��������
	void CreateWindowSizeDependentResources();	//	�E�B���h�E�T�C�Y�Ɉˑ����鏉��������
};

