/**********************************************
 *
 *  TitleScene.cpp
 *  �^�C�g���V�[��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "TitleScene.h"
#include "InputManager/InputManager.h"
#include "Utilities/Screen.h"
#include "WICTextureLoader.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	���S�̃p�X
const wchar_t* const TitleScene::LOGO_TEXTURE_PATH = L"Resources/Textures/logo.png";
//	���S�̃T�C�Y
const SimpleMath::Vector2 TitleScene::LOGO_SIZE = SimpleMath::Vector2(786, 354);


//	�R���X�g���N�^
TitleScene::TitleScene():
	Scene(Scenes::TITLE),
	m_logoProgress(0.0f)
{
	SetNextScene(Scenes::PLAY);
}

//	�f�X�g���N�^
TitleScene::~TitleScene()
{
}


//--------------------------------------------------------------------------------
// ����������
//--------------------------------------------------------------------------------
void TitleScene::Initialize()
{
}

//--------------------------------------------------------------------------------
// �X�V����
//--------------------------------------------------------------------------------
void TitleScene::Update(const float& deltaTime)
{
	if (InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Enter))
		ChangeScene();

	//	�{�^���̍X�V
	m_startButton->Update();
	m_exitButton->Update();

	//	���S�̍X�V
	LogoUpdate(deltaTime);
}

//--------------------------------------------------------------------------------
// �`�揈��
//--------------------------------------------------------------------------------
void TitleScene::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	�{�^���̕`��
	m_startButton->Render(context, *states);
	m_exitButton->Render(context, *states);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	m_spriteBatch->Draw(m_logoTexture.Get(), m_logoPos, nullptr);
	m_spriteBatch->End();
}

//--------------------------------------------------------------------------------
// �I������
//--------------------------------------------------------------------------------
void TitleScene::Finalize()
{
}

//--------------------------------------------------------------------------------
// �f�o�C�X�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void TitleScene::CreateDeviceDependentResources()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	�X�v���C�g�o�b�`�̍쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	//	���S�̓ǂݍ���
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, LOGO_TEXTURE_PATH, nullptr, m_logoTexture.ReleaseAndGetAddressOf())
	);

	//	�X�^�[�g�{�^���̍쐬
	m_startButton = std::make_unique<Button>(
		device,
		context,
		Screen::SCREEN_CENTER() + SimpleMath::Vector2(0.0f, 200.0f),
		SimpleMath::Vector2(450.0f, 127.5f),
		[&] {SetNextScene(Scenes::PLAY); ChangeScene(); },
		L"Resources/Textures/start_button.png"
	);
	//	�I���{�^���̍쐬
	m_exitButton = std::make_unique<Button>(
		device,
		context,
		Screen::SCREEN_CENTER() + SimpleMath::Vector2(0, 350),
		SimpleMath::Vector2(450.0f, 127.5),
		[&]() {GameExit(); },
		L"Resources/Textures/exit_button.png"
	);

}

//--------------------------------------------------------------------------------
// �E�B���h�E�T�C�Y�Ɉˑ����鏉��������
//--------------------------------------------------------------------------------
void TitleScene::CreateWindowSizeDependentResources()
{
}


//--------------------------------------------------------------------------------
// ���S�̍X�V����
//--------------------------------------------------------------------------------
void TitleScene::LogoUpdate(const float& deltaTime)
{
	const float AMPLITUDE = 30.0f;		//	�U�ꕝ
	const float SPEED = 3.0f;			//	�U�����x
	const float OFFSET_Y = 150.0f;		//	�S�̂̃Y���iY)

	//	sin�g�̌v�Z
	m_logoProgress = Repeat(m_logoProgress + deltaTime * SPEED, XM_2PI);
	float sinPos = (sinf(m_logoProgress) + 1.0f) * AMPLITUDE;

	//	���S�̍��W���X�V
	m_logoPos = Screen::SCREEN_CENTER();
	m_logoPos.x -= LOGO_SIZE.x / 2;
	m_logoPos.y -= LOGO_SIZE.y / 2 + OFFSET_Y + sinPos;
}