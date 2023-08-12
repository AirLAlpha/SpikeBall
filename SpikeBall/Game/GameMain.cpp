/**********************************************
 *
 *  GameMain.cpp
 *  ゲームのメインとなる部分を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "GameMain.h"

#include "InputManager/InputManager.h"

//	各シーンのヘッダファイル
#include "Scenes/PlayScene/PlayScene.h"
#include "Scenes/TitleScene/TitleScene.h"

//	シーン
const Scene::Scenes START_SCENE = Scene::Scenes::TITLE;		//	開始時のシーン


GameMain::GameMain():
	m_currentScene(nullptr)
{
}

GameMain::~GameMain()
{
	//	シーンが存在するときは終了処理を行い開放する
	if (m_currentScene != nullptr)
	{
		m_currentScene->Finalize();
		delete m_currentScene;
	}
}

//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
void GameMain::Initialize()
{
	ChangeScene(START_SCENE);					//	シーンの初期化

	CreateDeviceDependentResources();			//	デバイスに依存する初期化
	CreateWindowSizeDependentResources();		//	ウィンドウサイズに依存する初期化

	//	乱数の初期化
	srand((int)time(0));
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void GameMain::Update(const float& deltaTime)
{
	//	シーンが存在しないときは処理しない
	if (m_currentScene == nullptr) 
		return;

	//	現在のシーンの更新処理
	m_currentScene->Update(deltaTime);

	//	シーンの遷移確認
	bool changeSceneFlag = m_currentScene->GetChangeSceneFlag();
	//	遷移先が設定されたらシーンの遷移を実行する
	if (changeSceneFlag)
	{
		Scene::Scenes nextScene = m_currentScene->GetNextScene();
		ChangeScene(nextScene);
	}

	//	終了フラグの取得
	bool exitFlag = m_currentScene->GetGameExitFlag();
	//	終了フラグが有効になったらゲームを終了する
	if (exitFlag ||
		InputManager::GetInstance()->GetKeyDown(InputManager::Keys::Escape))
	{
		PostQuitMessage(0);
	}
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void GameMain::Render()
{
	//	シーンが存在しないときは処理しない
	if (m_currentScene == nullptr) 
		return;

	//	現在のシーンの描画処理
	m_currentScene->Render();
}

//--------------------------------------------------------------------------------
// 終了処理
//--------------------------------------------------------------------------------
void GameMain::Finalize()
{

}


//--------------------------------------------------------------------------------
// シーンの切り替え処理
//--------------------------------------------------------------------------------
void GameMain::ChangeScene(Scene::Scenes nextScene)
{
	//	遷移先のシーンが指定されていることを前提とする
	assert(nextScene != Scene::Scenes::NONE);

	//	シーンが存在するときは終了処理を行い開放する
	if (m_currentScene != nullptr)
	{
		m_currentScene->Finalize();
		delete m_currentScene;
	}

	//	遷移先のシーンを作成する
	switch (nextScene)
	{
	//	タイトルシーン
	case Scene::Scenes::TITLE:
		m_currentScene = new TitleScene;
		break;

	//	プレイシーン
	case Scene::Scenes::PLAY:
		m_currentScene = new PlayScene;
		break;

	default:
		return;
	}

	//	シーンが存在しないときはエラー
	assert(m_currentScene != nullptr);

	//	遷移先のシーンの初期化処理
	m_currentScene->Initialize();
	m_currentScene->CreateDeviceDependentResources();
	m_currentScene->CreateWindowSizeDependentResources();
}

//--------------------------------------------------------------------------------
// デバイスに依存する初期化処理
//--------------------------------------------------------------------------------
void GameMain::CreateDeviceDependentResources()
{
	//	現在のシーンの初期化
	if (m_currentScene != nullptr)
		m_currentScene->CreateDeviceDependentResources();
}

//--------------------------------------------------------------------------------
// ウィンドウサイズに依存する初期化処理
//--------------------------------------------------------------------------------
void GameMain::CreateWindowSizeDependentResources()
{
	//	現在のシーンの初期化
	if (m_currentScene != nullptr)
		m_currentScene->CreateWindowSizeDependentResources();
}
