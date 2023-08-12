/**********************************************
 *
 *  DebugCamera.h
 *  DebugCamera.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#pragma once

#include "../Camera.h"

class DebugCamera : public Camera
{
private:
	float sensitivity;							//	マウス感度

	DirectX::SimpleMath::Vector2 angle;			//	回転量
	DirectX::SimpleMath::Vector2 oldMousePos;	//	前回処理時のカーソル座標

	int wheelVal;								//	ホイールの回転量

private:
	void RotateUpdate(int x, int y);			//	回転の更新処理
	void MatrixUpdate();						//	行列の更新処理

public:
	DebugCamera();
	~DebugCamera();

	void Update();		//	更新処理

};
