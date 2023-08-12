/**********************************************
 *
 *  Button.cpp
 *  UIのボタンに関する処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "Button.h"

#include <WICTextureLoader.h>
#include "DeviceResources.h"
#include "InputManager/InputManager.h"

using namespace DirectX;


//	コンストラクタ
Button::Button(
	ID3D11Device* device,
	ID3D11DeviceContext* context,
	const SimpleMath::Vector2& pos,
	const SimpleMath::Vector2& size, 
	std::function<void(void)> func, 
	const wchar_t * texturePath
):
	m_pos(pos),
	m_size (size),
	m_pressedFunction (func),
	m_color (Colors::White)
{
	//	シェイプレンダーの作成
	m_shapeRender = std::make_unique<ShapeRender2D>(device, context);

	//	テクスチャの有効化フラグ
	bool enableTexture = texturePath != nullptr;
	if (!enableTexture)
		return;

	//	テクスチャの読み込み
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			texturePath,
			nullptr,
			m_texture.ReleaseAndGetAddressOf()
		)
	);
	//	シェイプレンダーに設定
	m_shapeRender->SetTextureEnable(device, true);
	m_shapeRender->SetTexture(m_texture.Get());
}

//	デストラクタ
Button::~Button()
{
}


//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Button::Update()
{
	auto mouseState = InputManager::GetInstance()->GetMouse()->GetState();
	auto bst = InputManager::GetInstance()->GetMouseButtonStateTracker();

	SimpleMath::Vector2 pos1 = m_pos - m_size / 2;
	SimpleMath::Vector2 pos2 = m_pos + m_size / 2;
	;
	if (pos1.x < mouseState.x && pos2.x > mouseState.x &&
		pos1.y < mouseState.y && pos2.y > mouseState.y)
	{
		//	カーソルがボタン上にある時
		m_color = Colors::Red;

		//	クリックされている時は色を暗くする
		if (mouseState.leftButton)
			m_color = Colors::DarkRed;

		//	クリックを離した時に処理を実行
		if (m_pressedFunction && bst->leftButton == Mouse::ButtonStateTracker::RELEASED )
		{
			m_pressedFunction();
		}

	}
	else
	{
		m_color = Colors::White;
	}
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Button::Render(ID3D11DeviceContext* context, const CommonStates& states)
{
	m_shapeRender->Begin(context, states);

	SimpleMath::Vector2 p1 = m_pos - m_size / 2;
	SimpleMath::Vector2 p2 = m_pos + m_size / 2;
	m_shapeRender->DrawBox(p1, p2, m_color, SimpleMath::Vector2(1.0f,1.0f));

	m_shapeRender->End();
}
