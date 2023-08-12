/**********************************************
 *
 *  TitleScene.cpp
 *  タイトルシーン
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "TitleScene.h"
#include "InputManager/InputManager.h"
#include "Utilities/Screen.h"
#include "WICTextureLoader.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	ロゴのパス
const wchar_t* const TitleScene::LOGO_TEXTURE_PATH = L"Resources/Textures/logo.png";
//	ロゴのサイズ
const SimpleMath::Vector2 TitleScene::LOGO_SIZE = SimpleMath::Vector2(786, 354);


//	コンストラクタ
TitleScene::TitleScene():
	Scene(Scenes::TITLE),
	m_logoProgress(0.0f)
{
	SetNextScene(Scenes::PLAY);
}

//	デストラクタ
TitleScene::~TitleScene()
{
}


//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
void TitleScene::Initialize()
{
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void TitleScene::Update(const float& deltaTime)
{
	if (InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Enter))
		ChangeScene();

	//	ボタンの更新
	m_startButton->Update();
	m_exitButton->Update();

	//	ロゴの更新
	LogoUpdate(deltaTime);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void TitleScene::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	ボタンの描画
	m_startButton->Render(context, *states);
	m_exitButton->Render(context, *states);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
	m_spriteBatch->Draw(m_logoTexture.Get(), m_logoPos, nullptr);
	m_spriteBatch->End();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void TitleScene::Finalize()
{
}

//--------------------------------------------------------------------------------
// デバイスに依存する初期化処理
//--------------------------------------------------------------------------------
void TitleScene::CreateDeviceDependentResources()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	//	ロゴの読み込み
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, LOGO_TEXTURE_PATH, nullptr, m_logoTexture.ReleaseAndGetAddressOf())
	);

	//	スタートボタンの作成
	m_startButton = std::make_unique<Button>(
		device,
		context,
		Screen::SCREEN_CENTER() + SimpleMath::Vector2(0.0f, 200.0f),
		SimpleMath::Vector2(450.0f, 127.5f),
		[&] {SetNextScene(Scenes::PLAY); ChangeScene(); },
		L"Resources/Textures/start_button.png"
	);
	//	終了ボタンの作成
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
// ウィンドウサイズに依存する初期化処理
//--------------------------------------------------------------------------------
void TitleScene::CreateWindowSizeDependentResources()
{
}


//--------------------------------------------------------------------------------
// ロゴの更新処理
//--------------------------------------------------------------------------------
void TitleScene::LogoUpdate(const float& deltaTime)
{
	const float AMPLITUDE = 30.0f;		//	振れ幅
	const float SPEED = 3.0f;			//	振幅速度
	const float OFFSET_Y = 150.0f;		//	全体のズレ（Y)

	//	sin波の計算
	m_logoProgress = Repeat(m_logoProgress + deltaTime * SPEED, XM_2PI);
	float sinPos = (sinf(m_logoProgress) + 1.0f) * AMPLITUDE;

	//	ロゴの座標を更新
	m_logoPos = Screen::SCREEN_CENTER();
	m_logoPos.x -= LOGO_SIZE.x / 2;
	m_logoPos.y -= LOGO_SIZE.y / 2 + OFFSET_Y + sinPos;
}