/**********************************************
 *
 *  Timer.h
 *  Timer.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#pragma once
#include "SpriteNumber/SpriteNumber.h"

class Timer : public SpriteNumber
{
private:
	//	時間の最大値
	static const float	TIME_MAX;
	//	アウトラインの幅
	static const int	OUTLINE_WIDTH;

private:
	//	描画位置
	DirectX::SimpleMath::Vector2	m_pos;

	//	時間
	float m_time;

	//	停止フラグ
	bool m_isStop;

public:
	//	コンストラクタ
	Timer(ID3D11Device* device, ID3D11DeviceContext* context, const DirectX::SimpleMath::Vector2& pos);
	//	デストラクタ
	~Timer();

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(const DirectX::CommonStates& states);

	//	リセット
	void Reset();

public:
	//	停止の設定
	void SetIsStop(bool stop) { m_isStop = stop; }
	//	座標の設定
	void SetPosition(const DirectX::SimpleMath::Vector2& pos) { m_pos = pos; }
	
public:
	//	時間の取得
	float GetTime() { return m_time; }
};
