/**********************************************
 *
 *  SphereCollider.cpp
 *  球状のコライダー
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/28
 *
 **********************************************/
#include "pch.h"
#include "SphereCollider.h"
#include "DebugDraw/DebugDraw.h"
#include "Colliders/PlaneCollider/PlaneCollider.h"
#include "Colliders/Line.h"

using namespace DirectX;


//	コンストラクタ
SphereCollider::SphereCollider(
	const SimpleMath::Vector3& pos,
	const float& radius
):
	m_pos(pos),
	m_radius(radius)
{
}

//	デストラクタ
SphereCollider::~SphereCollider()
{
}

//--------------------------------------------------------------------------------
// 無限平面との衝突判定
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const SimpleMath::Plane& plane) const
{
	//	平面までの距離を取得
	float dist = plane.DotCoordinate(m_pos);
	//	半径が平面までの距離以下なら衝突している
	if (fabsf(dist) < m_radius)
		return true;

	return false;
}

//--------------------------------------------------------------------------------
// 線分との衝突判定
// 参考	：http://marupeke296.com/COL_3D_No1_PointToLine.html
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const Line& line) const
{
	//	線分のベクトルを取得
	SimpleMath::Vector3 lineVec = line.GetNormalizedVector();
	//	線分の長さを取得
	float lineLength = line.GetLength();
	//	線分の始点と自身の座標とのベクトル
	SimpleMath::Vector3 lineToPos = m_pos - line.GetPosA();

	//	線分のベクトル（正規化済み）と線分の頂点から自信までのベクトルの内積を計算
	//	計算した内積を長さで割って、線分上の割合を計算
	float t = lineVec.Dot(lineToPos) / lineLength;
	if (t > 1.0f || t < 0.0f)
	{
		//	線分の上にないときは線分の頂点との距離から判定する
		SimpleMath::Vector3 lineVert;
		//	近い頂点を取得
		if (t > 1.0)
			lineVert = line.GetPosB();
		else
			lineVert = line.GetPosA();

		//	座標と頂点の距離の2乗を計算
		float dist = SimpleMath::Vector3::DistanceSquared(lineVert, m_pos);
		//	距離が半径の2乗より小さいときは衝突しているとする
		if (dist <= m_radius * m_radius)
			return true;
		else
			return false;
	}

	//	割合から座標を計算し、自身との距離を取得
	float h = (line.GetVector() * t - lineToPos).LengthSquared();
	//	距離が半径より大きいときは衝突していないとして終了
	if (h > m_radius * m_radius)
		return false;

	//	すべての条件をパスしたオブジェクトは衝突したとして終了
	return true;
}

//--------------------------------------------------------------------------------
// スフィアコライダー同士の衝突判定
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const SphereCollider& other) const
{
	//	戻り値
	bool ret = false;

	//	対象となるオブジェクトへのベクトルを作成
	SimpleMath::Vector3 toOtherVec = other.m_pos - m_pos;
	//	ベクトルの長さの二乗を計算
	float distSquare = toOtherVec.x * toOtherVec.x + toOtherVec.y * toOtherVec.y + toOtherVec.z * toOtherVec.z;
	//	衝突したとする距離の計算
	float hitDistSquare = (m_radius + other.m_radius) * (m_radius + other.m_radius);

	//	ベクトルの長さが衝突したとする距離より短いときは衝突しているとする
	if (distSquare <= hitDistSquare)
		ret = true;

	return ret;
}


#if _DEBUG
//--------------------------------------------------------------------------------
// デバッグ用の初期化処理
//--------------------------------------------------------------------------------
void SphereCollider::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
}

//--------------------------------------------------------------------------------
// デバッグ用の描画処理
//--------------------------------------------------------------------------------
void SphereCollider::DebugRender(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	インプットレイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	//	行列の設定
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	//	シェーダーの適応
	m_basicEffect->Apply(context);

	//	深度バッファを使用するように設定
	context->OMSetDepthStencilState(states.DepthDefault(), 0);

	//	枠の描画
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),
		DirectX::BoundingSphere(m_pos, m_radius),
		Colors::LightGreen
	);
	m_primitiveBatch->End();

}
#endif