/**********************************************
 *
 *  Ball.cpp
 *  ボール
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#include "pch.h"
#include "Ball.h"
#include "UserResources.h"
#include "Floor/Floor.h"

using namespace DirectX;

//	重力
const float Ball::GRAVITY = 0.3f;
//	速度調整用
const float Ball::SPEED_MOD = 0.1f;



//	コンストラクタ
Ball::Ball(ID3D11Device* device, const SimpleMath::Vector3& pos, const float radius) :
	m_pos(pos + SimpleMath::Vector3::Right * 0.0001f),
	m_radius(radius),
	m_onGround(false)
{
	//	エフェクトファクトリーの作成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	//	モデルの作成
	m_model = Model::CreateFromCMO(device, L"Resources/Models/ball.cmo", *fx);

	//	コライダーの初期化
	m_collider = std::make_unique<SphereCollider>(pos, radius);
}

//	デストラクタ
Ball::~Ball()
{
}


//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Ball::Update(const float& deltaTime)
{
	//	移動処理
	MoveUpdate(deltaTime);

	//	コライダーの座標を更新
	m_collider->SetPosition(m_pos);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Ball::Render(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	ワールド行列の作成
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateScale(m_radius) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	描画
	m_model->Draw(context, states, world, view, proj);
}

//--------------------------------------------------------------------------------
// 床との衝突判定
//--------------------------------------------------------------------------------
void Ball::CheckHitFloor(const Floor& floor)
{
	SimpleMath::Vector3 hitResult;
	if (floor.GetCollider()->CheckHit(*m_collider, &hitResult))
	{
		//	床の法線を取得
		m_groundNormal = floor.GetCollider()->GetNormal();
		//	床の角度を取得
		m_groundAngle = floor.GetAngle();

		//	衝突した点から法線方向に半径分押し戻す
		auto newPos = hitResult + m_groundNormal * m_radius;

		//	新しい座標を代入
		m_pos = newPos;
		m_collider->SetPosition(newPos);

		//	接地フラグを有効化
		m_onGround = true;
		return;
	}

	//	衝突していなければ、接地フラグを無効化
	m_onGround = false;
}


//--------------------------------------------------------------------------------
// 移動処理
//--------------------------------------------------------------------------------
void Ball::MoveUpdate(const float& deltaTime)
{
	if (m_onGround)
	{
		//	接地しているとき

		//	加速度との内積を取る
		float groundDot = m_acce.Dot(m_groundNormal);
		//	床の法線方向に対する加速度を打ち消す
		m_acce += m_groundNormal * -groundDot;

		//	地面の回転から目標となる加速度を作成
		SimpleMath::Vector3 forward = SimpleMath::Vector3::Forward * -m_groundAngle.x;
		SimpleMath::Vector3 right = SimpleMath::Vector3::Right * -m_groundAngle.y;

		//	目標となる加速度を正規化し、スピードを乗算
		SimpleMath::Vector3 targetAcce = (forward + right);
		targetAcce.Normalize();
		targetAcce *= SPEED_MOD;

		//	加速度を加算
		m_acce += targetAcce;
	}
	else
	{
		m_acce.y -= GRAVITY;
	}

	//	加速度を座標に加算
	m_pos += m_acce * deltaTime;

	//	移動量の計算
	float acceLength = m_acce.Length() * deltaTime * 1.5f;
	if (acceLength <= FLT_EPSILON)
	{
		m_rot = SimpleMath::Quaternion::Identity;
		return;
	}

	//	加速度から回転軸を作成
	SimpleMath::Vector3 axis = SimpleMath::Vector3(m_acce.z, m_acce.y, -m_acce.x);
	//	回転の作成
	SimpleMath::Quaternion deltaRot = SimpleMath::Quaternion::CreateFromAxisAngle(axis, acceLength);

	m_rot *= deltaRot;
}