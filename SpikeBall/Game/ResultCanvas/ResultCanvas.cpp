/**********************************************
 *
 *  ResultCanvas.cpp
 *  リザルトのUIに関する処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "ResultCanvas.h"

#include "WICTextureLoader.h"
#include "Utilities/Screen.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	テクスチャパス
const wchar_t*				ResultCanvas::RESULT_TEXTURE_PATH	= L"Resources/Textures/result.png";
const wchar_t*				ResultCanvas::TIME_TEXTURE_PATH		= L"Resources/Textures/time.png";
const SimpleMath::Vector2	ResultCanvas::EXIT_BUTTON_POS		= Screen::SCREEN_CENTER() + SimpleMath:: Vector2::UnitY * 340;

ResultCanvas::ResultCanvas(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	Scene::Scenes* nextScenePtr,
	bool* sceneChangeFlagPtr
):
	m_nextScenePtr(nextScenePtr),
	m_sceneChangeFlagPtr(sceneChangeFlagPtr)
{

	//	スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);
	//	シェイプレンダーの作成
	m_shapeRender = std::make_unique<ShapeRender2D>(device, context);

	//	キャンバスの座標を初期化
	m_canvasPos = SimpleMath::Vector2(0.0f, -Screen::HEIGHT);

	//	テクスチャの読み込み
	//	"RESULT"
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	CreateWICTextureFromFile(
		device,
		RESULT_TEXTURE_PATH,
		resource.ReleaseAndGetAddressOf(),
		m_resultTextrue.ReleaseAndGetAddressOf()
	);
	//テクスチャサイズの取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resource.As(&texture);
	CD3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	m_resultTextureSize.x = static_cast<float>(desc.Width);
	m_resultTextureSize.y = static_cast<float>(desc.Height);

	//	"TIME"
	CreateWICTextureFromFile(
		device,
		TIME_TEXTURE_PATH,
		resource.ReleaseAndGetAddressOf(),
		m_timeTextrue.ReleaseAndGetAddressOf()
	);
	//テクスチャサイズの取得
	resource.As(&texture);
	texture->GetDesc(&desc);
	m_timeTextureSize.x = static_cast<float>(desc.Width);
	m_timeTextureSize.y = static_cast<float>(desc.Height);

	//	ボタンの作成
	//	EXIT
	m_exitButton = std::make_unique<Button>(
		device,
		context,
		EXIT_BUTTON_POS + m_canvasPos,
		SimpleMath::Vector2(300, 85.0f),
		[&]() {*m_nextScenePtr = Scene::Scenes::TITLE; *m_sceneChangeFlagPtr = true; },
		L"Resources/Textures/exit_button.png");
	//	RETRY
	m_retryButton = std::make_unique<Button>(
		device,
		context,
		EXIT_BUTTON_POS + -SimpleMath::Vector2::UnitY * 120.0f + m_canvasPos,
		SimpleMath::Vector2(360, 102),
		[&]() {*m_nextScenePtr = Scene::Scenes::PLAY; *m_sceneChangeFlagPtr = true; },
		L"Resources/Textures/retry_button.png");

}

ResultCanvas::~ResultCanvas()
{

}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void ResultCanvas::Update(const float& deltaTime)
{
	m_canvasPos = SimpleMath::Vector2::Lerp(m_canvasPos, SimpleMath::Vector2::Zero, deltaTime * 10.0f);

	m_exitButton->Update();
	m_retryButton->Update();
	m_exitButton->SetPosition(EXIT_BUTTON_POS + m_canvasPos);
	m_retryButton->SetPosition(EXIT_BUTTON_POS + -SimpleMath::Vector2::UnitY * 120.0f + m_canvasPos);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void ResultCanvas::Render(ID3D11DeviceContext* context, const CommonStates& states)
{
	//	背景の描画
	m_shapeRender->Begin(context, states);
	m_shapeRender->DrawBox(SimpleMath::Vector2(50, 50) + m_canvasPos, Screen::SCREEN_SIZE() - SimpleMath::Vector2(50, 50) + m_canvasPos, {0,0,0,0.7f});
	m_shapeRender->End();

	//	"RESULT"と"TIME"の描画
	SimpleMath::Vector2 resultPos = SimpleMath::Vector2(static_cast<float>(Screen::WIDTH_HALF()), 200.0f) - m_resultTextureSize / 2.0f + m_canvasPos;
	SimpleMath::Vector2 timePos = SimpleMath::Vector2(static_cast<float>(Screen::WIDTH_HALF()), 400.0f) - m_timeTextureSize / 2.0f + m_canvasPos;
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	m_spriteBatch->Draw(m_resultTextrue.Get(), resultPos, nullptr);
	m_spriteBatch->Draw(m_timeTextrue.Get(), timePos, nullptr);
	m_spriteBatch->End();

	//	終了ボタンの描画
	m_exitButton->Render(context, states);
	//	リトライボタンの描画
	m_retryButton->Render(context, states);
}

