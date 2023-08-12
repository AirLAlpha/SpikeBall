/**********************************************
 *
 *  Scene.h
 *  各シーンの既定となるクラスを記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#pragma once
#include <SimpleMath.h>

#include "UserResources.h"

class Scene
{
public:
	enum class Scenes
	{
		NONE = 0,	//	遷移なし

		TITLE,		//	タイトル
		PLAY,		//	プレイ

		OVER_ID		//	範囲外
	};

private:
	Scenes	m_nowScene;			//	現在のシーン
	Scenes	m_nextScene;		//	遷移先のシーン
	bool	m_changeSceneFlag;	//	シーン遷移フラグ
	bool	m_gameExitFlag;		//	ゲームの終了フラグの取得

public:
	//	コンストラクタ
	Scene(Scenes nowScene) :
		m_nowScene(nowScene),
		m_nextScene(Scenes::NONE),
		m_changeSceneFlag(false),
		m_gameExitFlag(false)
	{
	};

	virtual ~Scene() {};				//	デストラクタ

	virtual void Initialize()					= 0;	//	初期化処理
	virtual void Update(const float& deltaTime)	= 0;	//	更新処理
	virtual void Render()						= 0;	//	描画処理
	virtual void Finalize()						= 0;	//	終了処理

	virtual void CreateDeviceDependentResources()		= 0;	//	デバイスに依存する初期化処理
	virtual void CreateWindowSizeDependentResources()	= 0;	//	ウィンドウサイズに依存する初期化

	void ChangeScene()	{ m_changeSceneFlag = true; }			//	シーン遷移の実行（フラグを有効化）
	void GameExit()		{ m_gameExitFlag = true; }				//	ゲームの終了（フラグを有効化）

	void SetNextScene(Scenes nextScene) { m_nextScene = nextScene; }	//	遷移先の設定

	Scenes	GetNextScene()			{ return m_nextScene; }				//	遷移先の取得
	Scenes* GetNextScenePtr()		{ return &m_nextScene; }			//	遷移先を示すポインタを取得
	bool	GetChangeSceneFlag()	{ return m_changeSceneFlag; }		//	シーン遷移フラグの取得
	bool*	GetChangeSceneFlagPtr()	{ return &m_changeSceneFlag; }		//	シーン遷移フラグを示すポインタの取得
	bool	GetGameExitFlag()		{ return m_gameExitFlag; }			//	ゲームの終了フラグの取得
};
