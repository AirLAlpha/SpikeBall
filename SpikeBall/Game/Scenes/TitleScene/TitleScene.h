/**********************************************
 *
 *  TitleScene.h
 *  TitleScene.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
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
	static const wchar_t* const					LOGO_TEXTURE_PATH;	//	ロゴのテクスチャパス
	static const DirectX::SimpleMath::Vector2	LOGO_SIZE;			//	ロゴのサイズ

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_logoTexture;		//	ロゴ
	DirectX::SimpleMath::Vector2						m_logoPos;			//	ロゴの座標
	float												m_logoProgress;		//	ロゴのsin用変数

	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;		//	スプライトバッチ

	std::unique_ptr<Button> m_startButton;	//	スタートボタン
	std::unique_ptr<Button> m_exitButton;	//	終了ボタン

private:
	//	ロゴの更新処理
	void LogoUpdate(const float& deltaTime);

public:
	TitleScene();
	~TitleScene();

	void Initialize()						override;		//	初期化処理
	void Update(const float& deltaTime)		override;		//	更新処理
	void Render()							override;		//	描画処理
	void Finalize()							override;		//	終了処理

	void CreateDeviceDependentResources() override;			//	デバイスに依存する初期化処理
	void CreateWindowSizeDependentResources() override;		//	ウィンドウサイズに依存する初期化処理
};


