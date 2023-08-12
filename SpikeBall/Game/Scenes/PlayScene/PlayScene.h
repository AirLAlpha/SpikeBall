/**********************************************
 *
 *  PlayScene.h
 *  PlayScene.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#pragma once
#include "../Scene.h"

#include "Camera/DebugCamera/DebugCamera.h"
#include "Camera/Camera.h"
#include "Floor/Floor.h"
#include "Ball/Ball.h"
#include "SpikeManager/SpikeManager.h"
#include "ResultCanvas/ResultCanvas.h"
#include "Timer/Timer.h"
#include "Background/Background.h"

class PlayScene : public Scene
{
private:
	//	死亡する高さ
	static const float DEAD_HEIGHT;

private:
	//	デバッグカメラ
	std::unique_ptr<DebugCamera>	m_debugCamera;
	//	カメラ
	std::unique_ptr<Camera>			m_camera;

	//	床
	std::unique_ptr<Floor>			m_floor;
	//	ボール
	std::unique_ptr<Ball>			m_ball;

	//	スパイクマネージャー
	std::unique_ptr<SpikeManager>	m_spikeManager;

	//	リザルト
	std::unique_ptr<ResultCanvas>	m_resultCanvas;
	//	タイマー
	std::unique_ptr<Timer>			m_timer;
	//	タイマー座標
	DirectX::SimpleMath::Vector2	m_timerPos;

	//	背景
	std::unique_ptr<Background>		m_background;

private:
	//	ゲームオーバー
	bool	m_isGameOver;


private:
	//	カメラの更新
	void CameraUpdate(const float& deltaTime);
	//	ゲームオーバー時の更新処理
	void GameOverUpdate(const float& deltaTime);

public:
	PlayScene();						//	コンストラクタ
	~PlayScene();						//	デストラクタ

	void Initialize()						override;		//	初期化処理
	void Update(const float& deltaTime)		override;		//	更新処理
	void Render()							override;		//	描画処理
	void Finalize()							override;		//	終了処理

	void CreateDeviceDependentResources() override;			//	デバイスに依存する初期化処理
	void CreateWindowSizeDependentResources() override;		//	ウィンドウサイズに依存する初期化処理
};