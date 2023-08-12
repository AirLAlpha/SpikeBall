/**********************************************
 *
 *  Floor.h
 *  Floor.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/27
 *
 **********************************************/
#pragma once
#include "Colliders/PlaneCollider/PlaneCollider.h"


class Floor
{
private:
	//	モデルのスケール
	static const DirectX::SimpleMath::Vector3	MODEL_SCALE;
	//	モデルのオフセット
	static const DirectX::SimpleMath::Vector3	MODEL_OFFSET;
	//	コライダーのスケール
	static const DirectX::SimpleMath::Vector2	COLLIDER_SCALE;
	//	回転速度
	static const float							ROTATE_SPEED;

private:
	//	モデル
	std::unique_ptr<DirectX::Model>		m_model;

	//	コライダー
	std::unique_ptr<PlaneCollider>		m_collider;

private:
	//	回転量
	DirectX::SimpleMath::Vector2		m_angle;
	//	回転
	DirectX::SimpleMath::Quaternion		m_rot;

private:
	//	回転処理
	void RotateUpdate(const float& deltaTime);

public:
	//	コンストラクタ
	Floor(ID3D11Device* device);
	//	デストラクタ
	~Floor();

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(
		ID3D11DeviceContext* context, 
		const DirectX::CommonStates& states, 
		const DirectX::SimpleMath::Matrix& view, 
		const DirectX::SimpleMath::Matrix& proj
	);

public:
	//	コライダーの取得
	PlaneCollider* GetCollider() const { return m_collider.get(); }
	//	回転量の取得
	DirectX::SimpleMath::Vector2 GetAngle() const { return m_angle; }

};
