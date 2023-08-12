/**********************************************
 *
 *  PlaneCollider.cpp
 *  板状のコライダー
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/29
 *
 **********************************************/
#include "pch.h"
#include "PlaneCollider.h"
#include "DebugDraw/DebugDraw.h"
#include "Colliders/SphereCollider/SphereCollider.h"
#include "Colliders/Line.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

#if _DEBUG
//	頂点座標
const SimpleMath::Vector3 PlaneCollider::VERTICIES[PlaneCollider::VERTEX_COUNT] =
{
	SimpleMath::Vector3(-0.5f, 0.0f, 0.5f),
	SimpleMath::Vector3(0.5f, 0.0f, 0.5f),
	SimpleMath::Vector3(0.5f, 0.0f, -0.5f),
	SimpleMath::Vector3(-0.5f, 0.0f, -0.5f)
};
#endif

//	コンストラクタ
PlaneCollider::PlaneCollider(
	const SimpleMath::Vector3& pos, 
	const SimpleMath::Vector2& size, 
	const SimpleMath::Quaternion& rot, 
	const SimpleMath::Vector3& normal
):
	m_pos(pos),
	m_size(size),
	m_rot(rot),
	m_normal(normal)
{
	//	平面の初期化
	m_plane = SimpleMath::Plane(pos, normal);
}

//	デストラクタ
PlaneCollider::~PlaneCollider()
{
}


//--------------------------------------------------------------------------------
// スフィアコライダーとの衝突判定
//--------------------------------------------------------------------------------
bool PlaneCollider::CheckHit(const SphereCollider& other, SimpleMath::Vector3* hitPoint) const
{
	//	無限平面に衝突しているかを確認する
	bool hitPlane = other.CheckHit(m_plane);
	//	無限平面に衝突していないときは衝突していないとして終了
	if (!hitPlane)
		return false;

	//	座標が全く同じ時は衝突しているとする
	if (Approximately(m_pos, other.GetPosition()))
		return true;

	//	平面の回転や拡縮の変換行列を作成
	SimpleMath::Matrix vMat =
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	otherを通る平面に垂直な線分の作成
	SimpleMath::Vector3 p1 = other.GetPosition() + m_normal * other.GetRadius();		//	線分の頂点1
	SimpleMath::Vector3 p2 = other.GetPosition() + -m_normal * other.GetRadius();		//	線分の頂点2
	Line l = Line(p1, p2);

	//	線分との衝突判定をとり衝突していたら終了
	if (CheckHit(l, hitPoint))
		return true;

	//	頂点
	const SimpleMath::Vector3 v[] =
	{
		SimpleMath::Vector3(-0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,0.5f),
		SimpleMath::Vector3(-0.5f,0.0f,0.5f),
	};
	//	辺と球の当たり判定
	for (int i = 0; i < 4; i++)
	{
		int index2 = Repeat(i + 1, 4);
		SimpleMath::Vector3 v1 = SimpleMath::Vector3::Transform(v[i], vMat);
		SimpleMath::Vector3 v2 = SimpleMath::Vector3::Transform(v[index2], vMat);

		//	線分のオブジェクトを生成
		Line l2 = Line(v1, v2);
		//	球との判定を取得
		bool edgeHit = other.CheckHit(l2);

		//	いずれかの線分と衝突していたらあたっているとして終了
		if (edgeHit)
			return true;
	}

	//	すべての条件に衝突しないときは、ポリゴンの内部に頂点が存在しないとして終了
	return false;
}

//--------------------------------------------------------------------------------
// 線分との衝突判定
//--------------------------------------------------------------------------------
bool PlaneCollider::CheckHit(const Line& line, SimpleMath::Vector3* hitPoint) const
{
	//	平面の回転や拡縮の変換行列を作成
	SimpleMath::Matrix vMat = 
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	SimpleMath::Vector3 p1 = line.GetPosA();
	SimpleMath::Vector3 p2 = line.GetPosB();

	//	平面の1頂点の座標を計算
	SimpleMath::Vector3 p0 = SimpleMath::Vector3::Transform(SimpleMath::Vector3(-0.5f, 0.0f, -0.5f), vMat);		//	平面の1頂点の座標

	//	平面の頂点から線分までのベクトルを作成
	SimpleMath::Vector3 v1 = p1 - p0;
	SimpleMath::Vector3 v2 = p2 - p0;

	//	線分の頂点から平面までの距離を取得
	float p1_dist = fabsf(m_plane.DotCoordinate(p1));
	float p2_dist = fabsf(m_plane.DotCoordinate(p2));

	//	内分点（交点）までの割合を計算
	float a = p1_dist / (p1_dist + p2_dist);
	float b = p2_dist / (p1_dist + p2_dist);

	//	交点の座標を計算
	SimpleMath::Vector3 v3 = (b * v1 + a * v2) + p0;

	if (hitPoint != nullptr)
		*hitPoint = v3;

	//	頂点
	const SimpleMath::Vector3 v[] =
	{
		SimpleMath::Vector3(-0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,0.5f),
		SimpleMath::Vector3(-0.5f,0.0f,0.5f),
	};
	//	インデックス
	const int indcies[] =
	{
		0, 1, 2,
		0, 2, 3,
	};
	//	1ポリゴンに対する頂点の数
	const int POLYGON_VERT_COUNT = 3;
	//	ポリゴンの数
	const int POLYGON_COUNT = 2;
	//	外積の結果を保持する変数
	SimpleMath::Vector3 crosses[POLYGON_VERT_COUNT];

	//	交点の座標が平面の内部にあるかを確認する
	for (int i = 0; i < POLYGON_COUNT; i++)
	{
		//	各頂点同士のベクトルと交点までのベクトルの外積を計算する
		for (int j = 0; j < POLYGON_VERT_COUNT; j++)
		{
			int index = j + i * POLYGON_VERT_COUNT;
			int index2 = Repeat(index + 1, POLYGON_VERT_COUNT) + i * POLYGON_VERT_COUNT;

			//	それぞれの頂点を平面の回転や大きさに合わせて移動させる
			SimpleMath::Vector3 v4 = SimpleMath::Vector3::Transform(v[indcies[index]], vMat);
			SimpleMath::Vector3 v5 = SimpleMath::Vector3::Transform(v[indcies[index2]], vMat);
			//	辺のベクトルを作成
			SimpleMath::Vector3 vertVec = v5 - v4;
			//	辺の頂点と交点までのベクトルを作成
			SimpleMath::Vector3 pointVec = v3 - v5;

			//	外積の計算を行う
			SimpleMath::Vector3 crossResult;
			pointVec.Cross(vertVec, crossResult);
			crossResult.Normalize();
			//	結果を保持
			crosses[j] = crossResult;
		}

		//	ポリゴンの内部に頂点が存在すれば衝突しているとして終了
		float d0 = SimpleMath::Vector3::DistanceSquared(crosses[0], m_normal);
		float d1 = SimpleMath::Vector3::DistanceSquared(crosses[1], m_normal);
		float d2 = SimpleMath::Vector3::DistanceSquared(crosses[2], m_normal);
		if (
			d0 <= FLT_EPSILON &&
			d1 <= FLT_EPSILON &&
			d2 <= FLT_EPSILON
			)
		{
			return true;
		}
	}

	//	ポリゴンの内部に交点が存在しないときは衝突していない
	return false;
}


#if _DEBUG
//--------------------------------------------------------------------------------
// デバッグ用初期化処理
//--------------------------------------------------------------------------------
void PlaneCollider::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
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
// デバッグ用描画処理
//--------------------------------------------------------------------------------
void PlaneCollider::DebugRender(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	インプットレイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	//	ワールド行列の作成
	SimpleMath::Matrix world = 
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) * 
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) * 
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	行列の設定
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	//	シェーダーの適応
	m_basicEffect->Apply(context);

	//	深度バッファを使用するように設定
	context->OMSetDepthStencilState(states.DepthDefault(), 0);

	//	枠の描画
	m_primitiveBatch->Begin();
	DX::DrawQuad(
		m_primitiveBatch.get(),
		VERTICIES[0],
		VERTICIES[1],
		VERTICIES[2],
		VERTICIES[3],
		Colors::LightGreen
	);
	m_primitiveBatch->End();

	//	ワールド行列をリセット
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->Apply(context);

	//	法線の描画
	m_primitiveBatch->Begin();
	DX::DrawRay(
		m_primitiveBatch.get(),
		m_pos,
		m_normal,
		false,
		Colors::LightGreen
	);
	m_primitiveBatch->End();
}
#endif