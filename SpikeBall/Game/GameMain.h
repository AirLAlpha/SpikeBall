/**********************************************
 *
 *  GameMain.h
 *  GameMain.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#pragma once
#include "Scenes/Scene.h"

class GameMain
{
private:
	Scene* m_currentScene;	//	現在のシーン

	

private:
	void ChangeScene(Scene::Scenes nextScene);		//	シーンの切り替え処理

public:
	GameMain();			//	コンストラクタ
	~GameMain();		//	デストラクタ

	void Initialize();							//	初期化処理
	void Update(const float& deltaTime);		//	更新処理
	void Render();								//	描画処理
	void Finalize();							//	終了処理

	void CreateDeviceDependentResources();		//	デバイスに依存する初期化処理
	void CreateWindowSizeDependentResources();	//	ウィンドウサイズに依存する初期化処理
};

