/**********************************************
 *
 *  PlayScene.cpp
 *  インゲーム部分の処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "PlayScene.h"
#include "InputManager/InputManager.h"
#include "Utilities/UtilityFunction.h"
#include "Utilities/Screen.h"

using namespace DirectX;

//	死亡する高さ
const float PlayScene::DEAD_HEIGHT = -15.0f;


//	コンストラクタ
PlayScene::PlayScene():
	Scene(Scenes::PLAY),
	m_isGameOver(false)
{
	SetNextScene(Scenes::TITLE);
}

//	デウストラクタ
PlayScene::~PlayScene()
{
}

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
void PlayScene::Initialize()
{
	//	デバッグカメラの作成
	m_debugCamera = std::make_unique<DebugCamera>();
	//	カメラの作成
	m_camera = std::make_unique<Camera>();
	m_camera->SetPosition(SimpleMath::Vector3(0.0f, 15.0f, 10.0f));
	m_camera->SetFov(0.01f);
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void PlayScene::Update(const float& deltaTime)
{
	//	デバッグカメラの更新
	m_debugCamera->Update();
	CameraUpdate(deltaTime);

	//	背景の更新
	m_background->Update(deltaTime);

	if (m_isGameOver)
	{
		GameOverUpdate(deltaTime);
		return;
	}

	//	床の更新
	m_floor->Update(deltaTime);
	//	ボールの更新
	m_ball->Update(deltaTime);
	//	スパイクマネージャーの更新
	m_spikeManager->Update(deltaTime);

	//	床との衝突判定
	m_ball->CheckHitFloor(*m_floor);

	//	棘と床との衝突判定
	m_spikeManager->CheckHitFloor(*m_floor);
	//	棘とプレイヤーとの衝突判定
	m_spikeManager->CheckHitBall(*m_ball);

	//	タイマーの更新
	m_timer->Update(deltaTime);

	//	一定以下の高さに行く or 棘に当たったらゲームオーバー
	if (m_ball->GetPosition().y < DEAD_HEIGHT ||
		m_spikeManager->GetIsHitPlayer())
	{
		m_isGameOver = true;
	}


	if (InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Enter))
		ChangeScene();
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void PlayScene::Render()
{
	//	必要なリソースの取得
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	カメラの行列を取得
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	//	シャドウマップへの描画
	m_shadowMap->Begin(context, *states);

	m_floor->ShadowMapRender(*m_shadowMap, context, *states);
	m_spikeManager->ShadowMapRender(*m_shadowMap, context);
	m_ball->ShadowMapRender(*m_shadowMap, context, *states);

	m_shadowMap->End(context);


	//	背景の描画
	m_background->Render(context, view, proj);
	//	床の描画
	m_floor->Render(context, *states, view, proj);
	//	ボールの描画
	m_ball->Render(context, *states, view, proj);

	//	スパイクの描画
	m_spikeManager->Render(view, proj);

	//	ゲームオーバー時はリザルトを描画
	if (m_isGameOver)
		m_resultCanvas->Render(context, *states);

	//	タイマーの描画
	m_timer->Render(*states);
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void PlayScene::Finalize()
{

}

//--------------------------------------------------------------------------------
// デバイスに依存する初期化処理
//--------------------------------------------------------------------------------
void PlayScene::CreateDeviceDependentResources()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	床の生成
	m_floor = std::make_unique<Floor>(device);
	//	ボールの生成
	m_ball = std::make_unique<Ball>(device, SimpleMath::Vector3::Up * 0.01f, 0.5f);

	//	スパイクマネージャーの作成
	m_spikeManager = std::make_unique<SpikeManager>(context);

	//	背景の生成
	m_background = std::make_unique<Background>(device, context);

	//	タイマーの生成
	m_timerPos = Screen::SCREEN_CENTER() + SimpleMath::Vector2(Screen::WIDTH / -4, -430);
	m_timer = std::make_unique<Timer>(device, context, m_timerPos);

	//	ライトの作成
	m_light = std::make_unique<DirectionalLight>(SimpleMath::Vector2(0.0f, XM_PI));
	m_floor->SetLight(m_light.get());

	//	シャドウマップの作成
	m_shadowMap = std::make_unique<ShadowMap>(*UserResources::GetInstance()->GetDeviceResources(), 512);
	m_shadowMap->SetLight(m_light.get());

	//	リザルトキャンバスの作成
	m_resultCanvas = std::make_unique<ResultCanvas>(device, context, GetNextScenePtr(), GetChangeSceneFlagPtr());
}

//--------------------------------------------------------------------------------
// ウィンドウサイズに依存する初期化処理
//--------------------------------------------------------------------------------
void PlayScene::CreateWindowSizeDependentResources()
{
}


//--------------------------------------------------------------------------------
// カメラの更新処理
//--------------------------------------------------------------------------------
void PlayScene::CameraUpdate(const float& deltaTime)
{
	//	ゲームオーバーじゃないとき
	if (!m_isGameOver)
	{
		float fov = Lerp(m_camera->GetFov(), 60.0f, deltaTime * 10.0f);
		m_camera->SetFov(fov);
	}
	else
	{
		//	ベクトルの計算
		SimpleMath::Vector3 playerPos = m_ball->GetPosition();
		SimpleMath::Vector3 toPlayerVec = playerPos - m_camera->GetPosition();
		toPlayerVec.Normalize();

		//	ターゲット座標
		SimpleMath::Vector3 targetPos = playerPos - toPlayerVec * 5.0f;

		//	徐々にプレイヤーに視点を合わせる
		SimpleMath::Vector3 lerpedPos = SimpleMath::Vector3::Lerp(m_camera->GetPosition(), targetPos, deltaTime * 10.0f);
		SimpleMath::Vector3 lerpedTarget = SimpleMath::Vector3::Lerp(m_camera->GetTarget(), playerPos, deltaTime * 10.0f);

		//	座標とターゲットを設定
		m_camera->SetPosition(lerpedPos);
		m_camera->SetTarget(lerpedTarget);
	}
}

//--------------------------------------------------------------------------------
// ゲームオーバー時の更新処理
//--------------------------------------------------------------------------------
void PlayScene::GameOverUpdate(const float& deltaTime)
{
	//	キャンバスの更新
	m_resultCanvas->Update(deltaTime);

	//	タイマーの座標を更新する
	m_timerPos = Screen::SCREEN_CENTER() + SimpleMath::Vector2(Screen::WIDTH / -4, 20) + m_resultCanvas->GetCanvasPos();
	m_timer->SetPosition(m_timerPos);
}
