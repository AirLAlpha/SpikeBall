/**********************************************
 *
 *  ResultCanvas.h
 *  ResultCanvas.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
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
	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;		//	スプライトバッチ
	std::unique_ptr<ShapeRender2D>			m_shapeRender;		//	図形の描画

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_resultTextrue;		//	"RESULT"のテクスチャ
	DirectX::SimpleMath::Vector2						m_resultTextureSize;	//	テクスチャサイズ

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_timeTextrue;		//	"TIME"のテクスチャ
	DirectX::SimpleMath::Vector2						m_timeTextureSize;	//	テクスチャサイズ

	std::unique_ptr<Button>	m_exitButton;			//	タイトルに戻るボタン
	std::unique_ptr<Button>	m_retryButton;			//	タイトルに戻るボタン

	DirectX::SimpleMath::Vector2 m_canvasPos;		//	キャンバスの基準座標

	Scene::Scenes*	m_nextScenePtr;
	bool*			m_sceneChangeFlagPtr;

private:
	

public:
	ResultCanvas(ID3D11Device* device, ID3D11DeviceContext* context, Scene::Scenes* nextScenePtr, bool* sceneChangeFlagPtr);
	~ResultCanvas();

	void Update(const float& deltaTime);		//	更新処理
	void Render(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	描画処理
	
	DirectX::SimpleMath::Vector2 GetCanvasPos() { return this->m_canvasPos; }
};
