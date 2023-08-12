/**********************************************
 *
 *  Camera.h
 *  Camera.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#pragma once

class Camera
{
private:
	//	デフォルトの値（定数）
	const float DEFAULT_FOV = 45.0f;		//	デフォルトの視野角
	const float DEFAULT_NEAR = 1.0f;		//	デフォルトのカリング距離（手前）
	const float DEFAULT_FAR = 100.0f;		//	デフォルトのカリング距離（奥）

private:
	//	View
	DirectX::SimpleMath::Vector3	m_pos;				//	座標
	DirectX::SimpleMath::Vector3	m_target;			//	注視点

	DirectX::SimpleMath::Vector3	m_up;				//	上とする向き

	//	Projection
	float							m_fieldOfView;		//	視野角
	float							m_nearPlane;		//	カリング距離（手前）
	float							m_farPlane;			//	カリング距離（奥）

	//	行列
	DirectX::SimpleMath::Matrix		m_view;				//	ビュー行列
	DirectX::SimpleMath::Matrix		m_projection;		//	射影行列

public:
	Camera();
	virtual ~Camera();

	//	ビュー行列の設定
	void SetView(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::Up);
	void SetView(DirectX::SimpleMath::Matrix view);

	//	射影行列の設定
	void SetProjection(float fieldOfView, float nearPlane, float farPlane);
	void SetProjection(DirectX::SimpleMath::Matrix projection);

	//	射影行列の再計算
	void CalculateProjectionMatrix();

	//	座標の設定
	void SetPosition(DirectX::SimpleMath::Vector3 pos);
	//	注視点の設定
	void SetTarget(DirectX::SimpleMath::Vector3 target);
	//	カリング距離の設定
	void SetCullingDist(float nearPlane, float farPlane);
	//	視野角の設定
	void SetFov(float fieldOfView);

	//	座標の取得
	DirectX::SimpleMath::Vector3 GetPosition();
	//	注視点の取得
	DirectX::SimpleMath::Vector3 GetTarget();
	//	視野角の取得
	float GetFov();

	//	ビュー行列の取得
	DirectX::SimpleMath::Matrix GetView();
	//	射影行列の取得
	DirectX::SimpleMath::Matrix GetProjection();

	//	正面ベクトルの取得
	DirectX::SimpleMath::Vector3 GetForward();
	//	右向きベクトルの取得
	DirectX::SimpleMath::Vector3 GetRight();
};

