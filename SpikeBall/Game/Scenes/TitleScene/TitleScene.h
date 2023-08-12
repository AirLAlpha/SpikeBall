/**********************************************
 *
 *  TitleScene.h
 *  TitleScene.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once
#include "Scenes/Scene.h"

#include "SpriteNumber/SpriteNumber.h"
#include "Timer/Timer.h"
#include "Button/Button.h"
#include "ResultCanvas/ResultCanvas.h"

class TitleScene : public Scene
{
private:
	static const wchar_t* const					LOGO_TEXTURE_PATH;	//	���S�̃e�N�X�`���p�X
	static const DirectX::SimpleMath::Vector2	LOGO_SIZE;			//	���S�̃T�C�Y

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_logoTexture;		//	���S
	DirectX::SimpleMath::Vector2						m_logoPos;			//	���S�̍��W
	float												m_logoProgress;		//	���S��sin�p�ϐ�

	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;		//	�X�v���C�g�o�b�`

	std::unique_ptr<Button> m_startButton;	//	�X�^�[�g�{�^��
	std::unique_ptr<Button> m_exitButton;	//	�I���{�^��

private:
	//	���S�̍X�V����
	void LogoUpdate(const float& deltaTime);

public:
	TitleScene();
	~TitleScene();

	void Initialize()						override;		//	����������
	void Update(const float& deltaTime)		override;		//	�X�V����
	void Render()							override;		//	�`�揈��
	void Finalize()							override;		//	�I������

	void CreateDeviceDependentResources() override;			//	�f�o�C�X�Ɉˑ����鏉��������
	void CreateWindowSizeDependentResources() override;		//	�E�B���h�E�T�C�Y�Ɉˑ����鏉��������
};


