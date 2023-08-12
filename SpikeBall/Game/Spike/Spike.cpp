/**********************************************
 *
 *  Spike.cpp
 *  棘の処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/13
 *
 **********************************************/
#include "pch.h"
#include "Spike.h"


using namespace DirectX;

//	半径
const float					Spike::RADIUS = 1.5f;
//	高さ
const float					Spike::HEIGHT = 2.0f;
//	回転
const SimpleMath::Vector3	Spike::ANGLE = SimpleMath::Vector3(XM_PI, 0.0f, 0.0f);
//	重力
const float					Spike::GRAVITY = 0.05f;
//	コライダーのオフセット
const SimpleMath::Vector3	Spike::COLLIDER_OFFSET = SimpleMath::Vector3::Up * 0.35f;


//	コンストラクタ
Spike::Spike(ID3D11DeviceContext* context):
	m_isEnable(false)
{
	//	ジオメトリックプリミティブの作成
	m_geometricPrimitive = GeometricPrimitive::CreateCone(context, RADIUS, HEIGHT);
	//	コライダーの作成
	m_collider = std::make_unique<SphereCollider>(COLLIDER_OFFSET, RADIUS / 3.0f);
}

//	デストラクタ
Spike::~Spike()
{
}


//--------------------------------------------------------------------------------
// 初期化処理
//--------------------------------------------------------------------------------
void Spike::Init(const DirectX::SimpleMath::Vector3& pos)
{
	m_pos = pos;
	m_acce = SimpleMath::Vector3::Zero;
	m_collider->SetPosition(m_pos);

	m_isEnable = true;
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Spike::Update(const float& deltaTime)
{
	//	重力を加算
	m_acce += SimpleMath::Vector3::Down * GRAVITY;

	//	加速度を座標に加算
	m_pos += m_acce * deltaTime;
	m_collider->SetPosition(m_pos + COLLIDER_OFFSET);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Spike::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	//	ワールド行列の作成
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateFromYawPitchRoll(ANGLE) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	描画
	m_geometricPrimitive->Draw(world, view, proj);
}
