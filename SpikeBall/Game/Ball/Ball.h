/**********************************************
 *
 *  Ball.h
 *  Ball.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#pragma once
#include "Colliders/SphereCollider/SphereCollider.h"

//	前方宣言
class Floor;


class Ball
{
private:
	//	重力
	static const float GRAVITY;
	//	速度調整用
	static const float SPEED_MOD;

private:
	//	モデル
	std::unique_ptr<DirectX::Model>		m_model;
	//	コライダー
	std::unique_ptr<SphereCollider>		m_collider;


private:
	//	座標
	DirectX::SimpleMath::Vector3		m_pos;
	//	加速度
	DirectX::SimpleMath::Vector3		m_acce;
	//	半径
	float								m_radius;
	//	回転
	DirectX::SimpleMath::Quaternion		m_rot;

	//	設置フラグ
	bool								m_onGround;

	//	床の法線
	DirectX::SimpleMath::Vector3		m_groundNormal;
	//	床の回転量
	DirectX::SimpleMath::Vector2		m_groundAngle;

private:
	//	移動処理
	void MoveUpdate(const float& deltaTime);

public:
	//	コンストラクタ
	Ball(
		ID3D11Device* device,
		const DirectX::SimpleMath::Vector3& pos,
		const float radius
	);
	//	デストラクタ
	~Ball();

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(
		ID3D11DeviceContext* context,
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//	床との衝突判定
	void CheckHitFloor(const Floor& floor);

public:
	//	座標の設定
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; m_collider->SetPosition(pos); }

public:
	//	座標の取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_pos; }
	//	半径の取得
	float GetRadius() const { return m_radius; }
	//	コライダーの取得
	SphereCollider* GetCollider() const { return m_collider.get(); }

};

