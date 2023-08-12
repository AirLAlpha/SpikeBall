/**********************************************
 *
 *  Button.h
 *  Button.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#pragma once

#include <memory>
#include <functional>
#include <SimpleMath.h>
#include "2DShapeRender/ShapeRender2D.h"

class Button
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		//	テクスチャ
	std::unique_ptr<ShapeRender2D>						m_shapeRender;	//	図形描画

	std::function<void(void)> m_pressedFunction;		//	選択時に呼び出す関数

	DirectX::SimpleMath::Vector2	m_pos;				//	座標
	DirectX::SimpleMath::Vector2	m_size;				//	サイズ
	DirectX::XMVECTORF32			m_color;			//	色

public:
	//	コンストラクタ
	Button(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const DirectX::SimpleMath::Vector2& pos,
		const DirectX::SimpleMath::Vector2& size,
		std::function<void(void)> func = NULL, 
		const wchar_t* texturePath = nullptr
	);
	//	デストラクタ
	~Button();

	void Update();		//	更新処理
	void Render(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	描画処理

	void SetPosition(const DirectX::SimpleMath::Vector2& pos)	{ m_pos = pos; }		//	座標の設定
	void SetSize(const DirectX::SimpleMath::Vector2& size)		{ m_size = size; }		//	サイズの設定
};
