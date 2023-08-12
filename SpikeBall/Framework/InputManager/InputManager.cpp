/**********************************************
 *
 *  InputManager.cpp
 *  入力全般の処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "InputManager.h"

using namespace DirectX;

InputManager::InputManager():
	m_keyboard{},
	m_keyboardStateTracker{},
	m_mouse{},
	m_buttonStateTracker{}
{
	//	各インスタンスの作成
	m_keyboard				= std::make_unique<DirectX::Keyboard>();
	m_keyboardStateTracker	= std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
	m_mouse					= std::make_unique<DirectX::Mouse>();
	m_buttonStateTracker	= std::make_unique<DirectX::Mouse::ButtonStateTracker>();
}

InputManager::~InputManager()
{
}


//--------------------------------------------------------------------------------
// インスタンスの取得
//--------------------------------------------------------------------------------
InputManager* InputManager::GetInstance()
{
	static InputManager instance;
	return &instance;
}

//--------------------------------------------------------------------------------
// 入力の更新処理　※Update内で一度だけ実行するように！
//--------------------------------------------------------------------------------
void InputManager::Update()
{
	//	キーボード
	auto keyState = m_keyboard->GetState();
	m_keyboardStateTracker->Update(keyState);

	//	マウス
	auto mouseState = m_mouse->GetState();
	m_buttonStateTracker->Update(mouseState);
}


//--------------------------------------------------------------------------------
// キーボードの取得
//--------------------------------------------------------------------------------
DirectX::Keyboard* InputManager::GetKeyboard()
{
	return m_keyboard.get();
}

//--------------------------------------------------------------------------------
// キーボードステートトラッカーの取得
//--------------------------------------------------------------------------------
DirectX::Keyboard::KeyboardStateTracker* InputManager::GetKeyboardStateTracker()
{
	return m_keyboardStateTracker.get();
}

//--------------------------------------------------------------------------------
// キーボードの入力を取得
//--------------------------------------------------------------------------------
DirectX::Keyboard::State InputManager::GetKeyboardState()
{
	return m_keyboard->GetState();
}

//--------------------------------------------------------------------------------
// キー入力を取得
//--------------------------------------------------------------------------------
bool InputManager::GetKey(Keyboard::Keys key)
{
	return m_keyboard->GetState().IsKeyDown(key);
}

//--------------------------------------------------------------------------------
// キーが入力された瞬間を取得
//--------------------------------------------------------------------------------
bool InputManager::GetKeyDown(Keyboard::Keys key)
{
	return m_keyboardStateTracker->IsKeyPressed(key);
}

//--------------------------------------------------------------------------------
// キーが離された瞬間を取得
//--------------------------------------------------------------------------------
bool InputManager::GetKeyUp(Keyboard::Keys key)
{
	return m_keyboardStateTracker->IsKeyReleased(key);
}

//--------------------------------------------------------------------------------
// マウスの取得
//--------------------------------------------------------------------------------
DirectX::Mouse* InputManager::GetMouse()
{
	return m_mouse.get();
}

//--------------------------------------------------------------------------------
// マウスボタンステートトラッカーの取得
//--------------------------------------------------------------------------------
DirectX::Mouse::ButtonStateTracker* InputManager::GetMouseButtonStateTracker()
{
	return m_buttonStateTracker.get();
}

//--------------------------------------------------------------------------------
// マウスのボタン入力を取得
//--------------------------------------------------------------------------------
bool InputManager::GetMouseButton(MouseButtons button)
{
	auto state = m_mouse->GetState();

	switch (button)
	{
	case InputManager::MouseButtons::LEFT:
		return  state.leftButton;
	case InputManager::MouseButtons::RIGHT:
		return state.rightButton;
	case InputManager::MouseButtons::MIDDLE:
		return state.middleButton;

	default:
		return false;
	}
}

//--------------------------------------------------------------------------------
// マウスのボタンが入力された瞬間を取得
//--------------------------------------------------------------------------------
bool InputManager::GetMouseButtonDown(MouseButtons button)
{
	auto state = m_buttonStateTracker.get();

	switch (button)
	{
	case InputManager::MouseButtons::LEFT:
		return state->leftButton == Mouse::ButtonStateTracker::PRESSED;

	case InputManager::MouseButtons::RIGHT:
		return state->rightButton == Mouse::ButtonStateTracker::PRESSED;

	case InputManager::MouseButtons::MIDDLE:
		return state->middleButton == Mouse::ButtonStateTracker::PRESSED;

	default:
		return false;
	}
}

//--------------------------------------------------------------------------------
// マウスのボタンが離された瞬間を取得
//--------------------------------------------------------------------------------
bool InputManager::GetMouseButtonUp(MouseButtons button)
{
	auto state = m_buttonStateTracker.get();

	switch (button)
	{
	case InputManager::MouseButtons::LEFT:
		return state->leftButton == Mouse::ButtonStateTracker::RELEASED;

	case InputManager::MouseButtons::RIGHT:
		return state->rightButton == Mouse::ButtonStateTracker::RELEASED;

	case InputManager::MouseButtons::MIDDLE:
		return state->middleButton == Mouse::ButtonStateTracker::RELEASED;

	default:
		return false;
	}
}