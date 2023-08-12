/**********************************************
 *
 *  SphereCollider.h
 *  SphereCollider.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/28
 *
 **********************************************/
#pragma once

//	前方宣言
class PlaneCollider;
class Line;

class SphereCollider
{
#if _DEBUG
private:
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;
	//	ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	m_primitiveBatch;
#endif
private:
	//	座標
	DirectX::SimpleMath::Vector3	m_pos;
	//	半径
	float							m_radius;


public:
	//	コンストラクタ
	SphereCollider(const DirectX::SimpleMath::Vector3& pos, const float& radius);
	//	デストラクタ
	~SphereCollider();

	//	無限平面との衝突判定
	bool CheckHit(const DirectX::SimpleMath::Plane& plane) const;
	//	線分との衝突判定
	bool CheckHit(const Line& line) const;
	//	スフィアコライダー同士の衝突判定
	bool CheckHit(const SphereCollider& other) const;


public:
	//	座標の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

public:
	//	座標の取得
	DirectX::SimpleMath::Vector3 GetPosition()	const { return m_pos; }
	//	半径の取得
	float GetRadius()					const {return m_radius;}

#if _DEBUG
public:
	//	デバッグ用の初期化処理
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	//	デバッグ用の描画処理
	void DebugRender(
		ID3D11DeviceContext* context,
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

#endif
};
