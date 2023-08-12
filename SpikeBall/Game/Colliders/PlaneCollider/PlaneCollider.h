/**********************************************
 *
 *  PlaneCollider.h
 *  PlaneCollider.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/29
 *
 **********************************************/
#pragma once

//	前方宣言
class SphereCollider;
class Line;


class PlaneCollider
{
#if _DEBUG
private:
	//	頂点の数
	static const int VERTEX_COUNT = 4;
	//	頂点
	static const DirectX::SimpleMath::Vector3 VERTICIES[VERTEX_COUNT];

private:
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;
	//	ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;
	
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	m_primitiveBatch;
#endif

private:
	//	無限平面
	DirectX::SimpleMath::Plane		m_plane;

	//	座標
	DirectX::SimpleMath::Vector3	m_pos;
	//	サイズ
	DirectX::SimpleMath::Vector2	m_size;
	//	回転
	DirectX::SimpleMath::Quaternion m_rot;
	//	法線
	DirectX::SimpleMath::Vector3	m_normal;


public:
	//	コンストラクタ
	PlaneCollider(
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector2& size,
		const DirectX::SimpleMath::Quaternion& rot,
		const DirectX::SimpleMath::Vector3& normal = DirectX::SimpleMath::Vector3::Up
	);
	//	デストラクタ
	~PlaneCollider();

public:
	//	座標の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)	{ m_pos = pos; m_plane = DirectX::SimpleMath::Plane(pos, m_normal); }
	//	サイズの設定
	void SetSize(const DirectX::SimpleMath::Vector2& size)		{ m_size = size; }
	//	回転の設定
	void SetRotate(const DirectX::SimpleMath::Quaternion& rot)	
	{ 
		m_rot = rot; 

		//	法線を回転させる
		m_normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, rot);
		m_plane = DirectX::SimpleMath::Plane(m_pos, m_normal);
	}

	//	スフィアコライダーとの衝突判定
	bool CheckHit(const SphereCollider& other, DirectX::SimpleMath::Vector3* hitPoint = nullptr) const;
	//	線分との衝突判定
	bool CheckHit(const Line& line, DirectX::SimpleMath::Vector3* hitPoint = nullptr) const;


public:
	//	無限平面の取得
	DirectX::SimpleMath::Plane GetPlane() const { return m_plane; }
	//	法線ベクトルの取得
	DirectX::SimpleMath::Vector3 GetNormal() const { return m_normal; }

#if _DEBUG
public:
	//	デバッグ用初期化処理
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	//	デバッグ用表示
	void DebugRender(
		ID3D11DeviceContext* context, 
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);
#endif
};

