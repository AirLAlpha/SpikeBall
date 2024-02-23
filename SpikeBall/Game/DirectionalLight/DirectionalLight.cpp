/**********************************************
 *
 *  DirectionalLight.cpp
 *  指向性ライト（太陽光）
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#include "pch.h"
#include "DirectionalLight.h"

using namespace DirectX;

//	定数の初期化
const float DirectionalLight::DEFAULT_DISTANCE = 20.0f;
const float DirectionalLight::DEFAULT_SIZE = 20.0f;
const float DirectionalLight::DEFAULT_NEAR = 1.0f;
const float DirectionalLight::DEFAULT_FAR = 100.0f;

//	コンストラクタ
DirectionalLight::DirectionalLight(const SimpleMath::Vector2& dir):
	m_dist(DEFAULT_DISTANCE),
	m_size(DEFAULT_SIZE)
{
	//	回転を設定
	SetAngle(dir);
}

//	デストラクタ
DirectionalLight::~DirectionalLight()
{
}


//--------------------------------------------------------------------------------
// ライトの方向を取得
//--------------------------------------------------------------------------------
SimpleMath::Vector3 DirectionalLight::GetDirection() const
{
	return SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Down, m_rot);
}

//--------------------------------------------------------------------------------
// ライトのビュー行列を取得
//--------------------------------------------------------------------------------
SimpleMath::Matrix DirectionalLight::GetLightView() const
{
	SimpleMath::Vector3 target = SimpleMath::Vector3::Zero;		//	注視点
	SimpleMath::Vector3 eye = GetDirection() * m_dist;			//	ライトの座標

	return SimpleMath::Matrix::CreateLookAt(eye, target, SimpleMath::Vector3::Up);
}

//--------------------------------------------------------------------------------
// ライトの射影行列を取得
//--------------------------------------------------------------------------------
SimpleMath::Matrix DirectionalLight::GetLightProjection() const
{
	return SimpleMath::Matrix::CreateOrthographic(m_size, m_size, DEFAULT_NEAR, DEFAULT_FAR);
}

//--------------------------------------------------------------------------------
// 方向の設定
//--------------------------------------------------------------------------------
void DirectionalLight::SetAngle(const SimpleMath::Vector2& angle)
{
	m_angle = angle;

	//	回転を設定
	m_rot =
		SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Right, m_angle.y) *
		SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, m_angle.x);
}


#if _DEBUG
#include <string>
#include "InputManager/InputManager.h"

//--------------------------------------------------------------------------------
// デバッグ用初期化処理
//--------------------------------------------------------------------------------
void DirectionalLight::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	デバッグ用フォントの作成
	m_debugFont = std::make_unique<Imase::DebugFont>(device, context, L"Resources/Fonts/SegoeUI_18.spritefont");
}
//--------------------------------------------------------------------------------
// デバッグ用移動処理
//--------------------------------------------------------------------------------
void DirectionalLight::DebugMoveUpdate(const float& deltaTime)
{
	//	キー入力で回転させる
	InputManager* input = InputManager::GetInstance();
	SimpleMath::Vector2 inputVec;
	if (input->GetKey(InputManager::Keys::W))
		inputVec.y += 1.0f;
	if (input->GetKey(InputManager::Keys::S))
		inputVec.y -= 1.0f;
	if (input->GetKey(InputManager::Keys::A))
		inputVec.x += 1.0f;
	if (input->GetKey(InputManager::Keys::D))
		inputVec.x -= 1.0f;
	m_angle += inputVec * deltaTime;
	SetAngle(m_angle);

	if (input->GetKey(InputManager::Keys::Q))
		m_size -= 0.1f;
	if (input->GetKey(InputManager::Keys::E))
		m_size += 0.1f;

}
//--------------------------------------------------------------------------------
// デバッグ用描画処理
//--------------------------------------------------------------------------------
void DirectionalLight::DebugRender(DirectX::CommonStates* commonStates)
{
	//	向きを string に変換
	std::wstring dirStr =
		L"x : " + std::to_wstring(m_angle.x) +
		L"  y : " + std::to_wstring(m_angle.y);

	//	方向を表示
	m_debugFont->AddString(dirStr.c_str(), { 5,5 }, Colors::Black);

	m_debugFont->Render(commonStates);
}
#endif
