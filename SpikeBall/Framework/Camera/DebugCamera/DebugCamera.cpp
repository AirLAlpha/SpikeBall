/**********************************************
 *
 *  DebugCamera.cpp
 *  デバッグ用のカメラの処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#include "pch.h"
#include "DebugCamera.h"

#include "UserResources.h"
#include "InputManager/InputManager.h"

using namespace DirectX;

const float DEFAULT_SENSITIVITY = 20.0f;
const float DEFAULT_DISTANCE = 2.0f;

DebugCamera::DebugCamera() :
	sensitivity(DEFAULT_SENSITIVITY),
	wheelVal(0)
{
	angle.x = XM_PI / 6 * 2;
	angle.y = 0;

}

DebugCamera::~DebugCamera()
{
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void DebugCamera::Update()
{
	//	マウスの入力を取得
	auto mouseState = InputManager::GetInstance()->GetMouse()->GetState();
	

	if (mouseState.leftButton)
		RotateUpdate(mouseState.x, mouseState.y);

	//	ホイールの回転量を保持
	wheelVal = mouseState.scrollWheelValue;
	if (mouseState.scrollWheelValue > 0)
	{
		wheelVal = 0;
		InputManager::GetInstance()->GetMouse()->ResetScrollWheelValue();
	}

	//	行列の更新処理
	MatrixUpdate();

	//	マウスの座標を保持
	oldMousePos.x = static_cast<float>(mouseState.x);
	oldMousePos.y = static_cast<float>(mouseState.y);
}


//--------------------------------------------------------------------------------
// 回転の更新処理
//--------------------------------------------------------------------------------
void DebugCamera::RotateUpdate(int x, int y)
{
	//	マウスの移動量を計算
	float deltaTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	float dx = (x - oldMousePos.x) * sensitivity * deltaTime;
	float dy = (y - oldMousePos.y) * sensitivity * deltaTime;

	//	回転量を計算
	float angleX = dy * XM_PI / 180.0f;
	float angleY = dx * XM_PI / 180.0f;

	//	回転量を加算
	angle += SimpleMath::Vector2(angleX, angleY);
}

//--------------------------------------------------------------------------------
// 行列の更新処理
//--------------------------------------------------------------------------------
void DebugCamera::MatrixUpdate()
{
	//	回転行列の作成
	SimpleMath::Matrix rot =
		SimpleMath::Matrix::CreateRotationY(angle.y) *
		SimpleMath::Matrix::CreateRotationX(angle.x);

	//	計算用の変数を作成
	SimpleMath::Vector3 pos = SimpleMath::Vector3(0, 0, 1);
	SimpleMath::Vector3 up = SimpleMath::Vector3::Up;

	//	行列を使って移動させる
	pos = SimpleMath::Vector3::Transform(pos, rot.Invert());
	pos *= DEFAULT_DISTANCE - wheelVal / 100;
	up = SimpleMath::Vector3::Transform(up, rot.Invert());

	////	移動させた変数を適応
	//this->m_pos = pos;
	//this->m_up = up;

	SimpleMath::Vector3 target = GetTarget();
	//	ビュー行列の更新
	SetView(target + pos, target, up);
}
