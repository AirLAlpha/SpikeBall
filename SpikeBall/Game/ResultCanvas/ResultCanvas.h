/**********************************************
 *
 *  ResultCanvas.h
 *  ResultCanvas.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/31
 *
 **********************************************/
#pragma once

#include "2DShapeRender/ShapeRender2D.h"
#include "Button/Button.h"
#include "Game/Scenes/Scene.h"

class ResultCanvas
{
private:
	static const wchar_t* RESULT_TEXTURE_PATH;
	static const wchar_t* TIME_TEXTURE_PATH;

	static const DirectX::SimpleMath::Vector2 EXIT_BUTTON_POS;

private:
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;		//	�X�v���C�g�o�b�`
	std::unique_ptr<ShapeRender2D>			m_shapeRender;		//	�}�`�̕`��

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_resultTextrue;		//	"RESULT"�̃e�N�X�`��
	DirectX::SimpleMath::Vector2						m_resultTextureSize;	//	�e�N�X�`���T�C�Y

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_timeTextrue;		//	"TIME"�̃e�N�X�`��
	DirectX::SimpleMath::Vector2						m_timeTextureSize;	//	�e�N�X�`���T�C�Y

	std::unique_ptr<Button>	m_exitButton;			//	�^�C�g���ɖ߂�{�^��
	std::unique_ptr<Button>	m_retryButton;			//	�^�C�g���ɖ߂�{�^��

	DirectX::SimpleMath::Vector2 m_canvasPos;		//	�L�����o�X�̊���W

	Scene::Scenes*	m_nextScenePtr;
	bool*			m_sceneChangeFlagPtr;

private:
	

public:
	ResultCanvas(ID3D11Device* device, ID3D11DeviceContext* context, Scene::Scenes* nextScenePtr, bool* sceneChangeFlagPtr);
	~ResultCanvas();

	void Update(const float& deltaTime);		//	�X�V����
	void Render(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	�`�揈��
	
	DirectX::SimpleMath::Vector2 GetCanvasPos() { return this->m_canvasPos; }
};
