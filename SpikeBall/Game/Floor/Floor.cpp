/**********************************************
 *
 *  Floor.cpp
 *  床に関する処理
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#include "pch.h"
#include "Floor.h"
#include "UserResources.h"
#include "InputManager/InputManager.h"

using namespace DirectX;

//	モデルのスケール
const SimpleMath::Vector3	Floor::MODEL_SCALE	= SimpleMath::Vector3::One * 1.5f;
//	モデルのオフセット
const SimpleMath::Vector3	Floor::MODEL_OFFSET	= SimpleMath::Vector3::Down * 0.375f;
//	コライダーのスケール
const SimpleMath::Vector2	Floor::COLLIDER_SCALE = SimpleMath::Vector2::One * 15.0f;
//	回転速度
const float					Floor::ROTATE_SPEED = 1.2f;


//	コンストラクタ
Floor::Floor(ID3D11Device* device)
{
	//	エフェクトファクトリーの作成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);

	//	モデルの読み込み
	m_model = Model::CreateFromCMO(device, L"Resources/Models/floor.cmo", *fx);

	//	コライダーの作成
	m_collider = std::make_unique<PlaneCollider>(
		SimpleMath::Vector3::Zero,
		COLLIDER_SCALE,
		m_rot
	);
}

//	デストラクタ
Floor::~Floor()
{
}


//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Floor::Update(const float& deltaTime)
{
	//	回転処理
	RotateUpdate(deltaTime);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Floor::Render(
	ID3D11DeviceContext* context, 
	const DirectX::CommonStates& states,
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj
)
{
	//	ワールド変換行列を計算
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateScale(MODEL_SCALE) *
		SimpleMath::Matrix::CreateTranslation(MODEL_OFFSET) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot);

	//	モデルの描画
	m_model->Draw(context, states, world, view, proj);

}



//--------------------------------------------------------------------------------
// 回転処理
//--------------------------------------------------------------------------------
void Floor::RotateUpdate(const float& deltaTime)
{
	//	入力マネージャーの取得
	auto input = InputManager::GetInstance();

	//	入力に応じて回転させる
	SimpleMath::Vector2 deltaAngle;
	if (input->GetKey(InputManager::Keys::W))
		deltaAngle.x += -deltaTime;
	if (input->GetKey(InputManager::Keys::S))
		deltaAngle.x += deltaTime;
	if (input->GetKey(InputManager::Keys::D))
		deltaAngle.y += -deltaTime;
	if (input->GetKey(InputManager::Keys::A))
		deltaAngle.y += deltaTime;

	//	回転量を加算
	m_angle += deltaAngle * ROTATE_SPEED;

	//	回転量からクォータニオンの作成
	m_rot = SimpleMath::Quaternion::CreateFromYawPitchRoll(SimpleMath::Vector3(m_angle.x, 0.0f, m_angle.y));
	//	コライダーに反映
	m_collider->SetRotate(m_rot);
}
