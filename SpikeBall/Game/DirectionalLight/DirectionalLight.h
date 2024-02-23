/**********************************************
 *
 *  DirectionalLight.h
 *  DirectionalLight.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/07/03
 *
 **********************************************/
#pragma once

#if _DEBUG
#include "ImaseLib/DebugFont/DebugFont.h"
#endif

class DirectionalLight
{
private:
	static const float DEFAULT_DISTANCE;		//	デフォルトの距離
	static const float DEFAULT_SIZE;			//	デフォルトのサイズ（範囲）
	static const float DEFAULT_NEAR;			//	デフォルトのニアクリップ
	static const float DEFAULT_FAR;				//	デフォルトのファークリップ

private:
	//	座標のオフセット
	DirectX::SimpleMath::Vector3	m_offset;
	//	向き
	DirectX::SimpleMath::Vector2	m_angle;
	//	回転
	DirectX::SimpleMath::Quaternion m_rot;
	//	距離
	float							m_dist;
	//	大きさ（範囲）
	float							m_size;


#if _DEBUG
	std::unique_ptr<Imase::DebugFont> m_debugFont;
#endif

public:
	//	コンストラクタ
	DirectionalLight(const DirectX::SimpleMath::Vector2& dir);
	//	デストラクタ
	~DirectionalLight();


#if _DEBUG
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	void DebugMoveUpdate(const float& deltaTime);
	void DebugRender(DirectX::CommonStates* commonStates);
#endif

public:
	//	方向の取得
	DirectX::SimpleMath::Vector3 GetDirection() const;
	//	角度の取得
	DirectX::SimpleMath::Vector2 GetAngle() const { return m_angle; }

	//	ビュー行列の取得
	DirectX::SimpleMath::Matrix GetLightView() const;

	//	射影行列の取得
	DirectX::SimpleMath::Matrix GetLightProjection() const;

	//	サイズの取得
	float GetLightSize() const { return m_size; } 

public:
	//	オフセットの設定
	void SetOffset(const DirectX::SimpleMath::Vector3& offset) { m_offset = offset; }
	//	方向の設定
	void SetAngle(const DirectX::SimpleMath::Vector2& angle);


};
