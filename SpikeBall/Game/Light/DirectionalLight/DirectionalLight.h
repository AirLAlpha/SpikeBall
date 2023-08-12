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
	void DebugRender(DirectX::CommonStates* commonStates);
#endif

public:
	//	方向の取得
	DirectX::SimpleMath::Vector3 GetDirection() const;

	//	ビュー行列の取得
	DirectX::SimpleMath::Matrix GetLightView() const;
	//	射影行列の取得
	DirectX::SimpleMath::Matrix GetLightProjection() const;

public:
	//	方向の設定
	void SetAngle(const DirectX::SimpleMath::Vector2& angle);


};
