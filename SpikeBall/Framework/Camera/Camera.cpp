/**********************************************
 *
 *  Camera.cpp
 *  基本的なカメラの処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/25
 *
 **********************************************/
#include "pch.h"
#include "Camera.h"

#include "DeviceResources.h"
#include "StepTimer.h"
#include "../Utilities/UtilityFunction.h"

#include "UserResources.h"

using namespace DirectX;

Camera::Camera():
	m_fieldOfView(DEFAULT_FOV),
	m_nearPlane(DEFAULT_NEAR),
	m_farPlane(DEFAULT_FAR),
	m_up(SimpleMath::Vector3::Up)
{
	CalculateProjectionMatrix();		//	デフォルトの射影行列を計算
}

Camera::~Camera()
{
}

//--------------------------------------------------------------------------------
// ビュー行列の設定
//--------------------------------------------------------------------------------
void Camera::SetView(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 up)
{
	//	各値を保持
	this->m_pos = pos;
	this->m_target = target;
	this->m_up = up;

	//	行列を作成
	m_view = SimpleMath::Matrix::CreateLookAt(pos, target, up);
}
//--------------------------------------------------------------------------------
// ビュー行列の設定（行列指定版）
//--------------------------------------------------------------------------------
void Camera::SetView(DirectX::SimpleMath::Matrix view)
{
	this->m_view = view;
}

//--------------------------------------------------------------------------------
// 射影行列の設定
//--------------------------------------------------------------------------------
void Camera::SetProjection(float fieldOfView, float nearPlane, float farPlane)
{
	//	各値を保持
	this->m_fieldOfView = fieldOfView;
	this->m_nearPlane = nearPlane;
	this->m_farPlane = farPlane;

	//	アスペクト比を計算
	auto deviceResources =  UserResources::GetInstance()->GetDeviceResources();
	LONG width = deviceResources->GetOutputSize().right;
	LONG height = deviceResources->GetOutputSize().bottom;
	float aspectRatio = (float)width / (float)height;

	//	行列の作成
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}
//--------------------------------------------------------------------------------
// 射影行列の設定（行列指定版）
//--------------------------------------------------------------------------------
void Camera::SetProjection(DirectX::SimpleMath::Matrix projection)
{
	this->m_projection = projection;
}

//--------------------------------------------------------------------------------
// 射影行列の再計算
//--------------------------------------------------------------------------------
void Camera::CalculateProjectionMatrix()
{
	//	アスペクト比を計算
	auto deviceResources = UserResources::GetInstance()->GetDeviceResources();
	LONG width = deviceResources->GetOutputSize().right;
	LONG height = deviceResources->GetOutputSize().bottom;
	float aspectRatio = (float)width / (float)height;

	//	FOVをラジアンに変換
	float radFov = XMConvertToRadians(m_fieldOfView);

	//	行列の作成
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(radFov, aspectRatio, m_nearPlane, m_farPlane);
}

//--------------------------------------------------------------------------------
// 座標の設定
//--------------------------------------------------------------------------------
void Camera::SetPosition(DirectX::SimpleMath::Vector3 pos)
{
	this->m_pos = pos;
	SetView(m_pos, m_target, m_up);
}

//--------------------------------------------------------------------------------
// 注視点の設定
//--------------------------------------------------------------------------------
void Camera::SetTarget(DirectX::SimpleMath::Vector3 target)
{
	this->m_target = target;
	SetView(m_pos, m_target, m_up);
}

//--------------------------------------------------------------------------------
// カリング距離の設定
//--------------------------------------------------------------------------------
void Camera::SetCullingDist(float nearPlane, float farPlane)
{
	this->m_nearPlane = nearPlane;
	this->m_farPlane = farPlane;

	//	行列の更新
	CalculateProjectionMatrix();
}

//--------------------------------------------------------------------------------
// 視野の設定
//--------------------------------------------------------------------------------
void Camera::SetFov(float fieldOfView)
{
	this->m_fieldOfView = fieldOfView;
	CalculateProjectionMatrix();
}

//--------------------------------------------------------------------------------
// 座標の取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetPosition()
{
	return m_pos;
}

//--------------------------------------------------------------------------------
// 注視点の取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetTarget()
{
	return m_target;
}

//--------------------------------------------------------------------------------
// 視野角の取得
//--------------------------------------------------------------------------------
float Camera::GetFov()
{
	return m_fieldOfView;
}

//--------------------------------------------------------------------------------
// ビュー行列の取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetView()
{
	return m_view;
}

//--------------------------------------------------------------------------------
// 射影行列の取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetProjection()
{
	return m_projection;
}

//--------------------------------------------------------------------------------
// 正面の向きベクトルの取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetForward()
{
	SimpleMath::Vector3 ret;
	ret = m_target - m_pos;
	ret.Normalize();

	return ret;
}

//--------------------------------------------------------------------------------
// 右向きベクトルの取得
//--------------------------------------------------------------------------------
DirectX::SimpleMath::Vector3 Camera::GetRight()
{
	SimpleMath::Vector3 forward;
	forward = GetForward();

	return SimpleMath::Vector3(-forward.z, 0.0f, forward.x);
}
