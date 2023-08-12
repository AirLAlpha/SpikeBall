/**********************************************
 *
 *  Spike.h
 *  Spike.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#pragma once
#include "Colliders/SphereCollider/SphereCollider.h"


class Spike
{
private:
	//	半径
	static const float RADIUS;
	//	高さ
	static const float HEIGHT;
	//	回転
	static const DirectX::SimpleMath::Vector3 ANGLE;

	//	重力
	static const float GRAVITY;

	//	コライダーのオフセット
	static const DirectX::SimpleMath::Vector3 COLLIDER_OFFSET;

private:
	//	ジオメトリックプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive>	m_geometricPrimitive;

	//	座標
	DirectX::SimpleMath::Vector3	m_pos;
	//	加速度
	DirectX::SimpleMath::Vector3	m_acce;

	//	コライダー
	std::unique_ptr<SphereCollider> m_collider;

	//	スパイクの有効化フラグ
	bool							m_isEnable;

public:
	//	コンストラクタ
	Spike(ID3D11DeviceContext* context);
	//	デストラクタ
	~Spike();

	//	初期化処理
	void Init(const DirectX::SimpleMath::Vector3& pos);

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

public:
	//	座標の取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_pos; }
	//	コライダーの取得
	SphereCollider* GetCollider() const { return m_collider.get(); }
	//	アクティブの取得
	bool GetIsEnable() const { return m_isEnable; }

public:
	//	アクティブの設定
	void SetIsEnable(const bool& enable) { m_isEnable = enable; }

};


