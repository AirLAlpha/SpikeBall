/**********************************************
 *
 *  InputManager.h
 *  InputManager.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#pragma once

#include <Keyboard.h>
#include <Mouse.h>

class InputManager
{
public:
	using Keys = DirectX::Keyboard::Keys;

	enum class MouseButtons
	{
		LEFT,		//	左
		RIGHT,		//	右
		MIDDLE,		//	ホイール
	};

private:
	//	キーボード
	std::unique_ptr<DirectX::Keyboard>							m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>	m_keyboardStateTracker;

	//	マウス
	std::unique_ptr<DirectX::Mouse>						m_mouse;
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_buttonStateTracker;

private:
	InputManager();		//	コンストラクタ
	~InputManager();	//	デストラクタ

public:
	//	コピーコンストラクタ、ムーブコンストラクタを削除
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

	static InputManager* GetInstance();		//	インスタンスの取得

public:
	void Update();		//	入力の更新処理　※Update内で一度だけ実行するように！

	DirectX::Keyboard*							GetKeyboard();								//	キーボードの取得
	DirectX::Keyboard::KeyboardStateTracker*	GetKeyboardStateTracker();					//	キーボードステートトラッカーの取得
	DirectX::Keyboard::State					GetKeyboardState();							//	キーボードの入力を取得
	bool										GetKey		(Keys key);						//	キー入力を取得
	bool										GetKeyDown	(Keys key);						//	キーが入力された瞬間を取得
	bool										GetKeyUp	(Keys key);						//	キーが離された瞬間を取得

	DirectX::Mouse*								GetMouse();									//	マウスの取得
	DirectX::Mouse::ButtonStateTracker*			GetMouseButtonStateTracker();				//	マウスボタンステートトラッカーの取得
	bool										GetMouseButton		(MouseButtons button);	//	マウスのボタンの入力を取得
	bool										GetMouseButtonDown	(MouseButtons button);	//	マウスのボタンが入力された瞬間を取得
	bool										GetMouseButtonUp	(MouseButtons button);	//	マウスのボタンが離された瞬間を取得
};
