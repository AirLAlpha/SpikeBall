/**********************************************
 *
 *  SpikeManager.h
 *  SpikeManager.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#pragma once
#include "Spike/Spike.h"

//	前方宣言
class Floor;
class Ball;


class SpikeManager
{
private:
	//	スパイクの総数
	static const int	SPIKE_COUNT = 30;
	//	スパイクの開始Y座標
	static const float	SPIKE_START_POS_Y;
	//	スパイクが削除されるY座標
	static const float	SPIKE_DELETE_POS_Y;
	//	スパイクの生成間隔
	static const float	SPIKE_GENERATE_INTERVAL;

private:
	//	スパイクの配列
	std::vector<std::unique_ptr<Spike>> m_spikeList;

	//	生成間隔
	float			m_generateInterval;

	//	プレイヤーとの衝突フラグ
	bool			m_isHitBall;

private:
	//	スパイクのランダム生成
	void GenerateSpike(const DirectX::SimpleMath::Vector3& range);
	//	スパイクの生成処理
	void GenerateUpdate(const float& deltaTime);

public:
	SpikeManager(ID3D11DeviceContext* context);
	~SpikeManager();

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//	床との衝突判定
	void CheckHitFloor(const Floor& floor);
	//	ボールとの衝突判定
	void CheckHitBall(const Ball& ball);

public:
	//	プレイヤーとの衝突フラグを取得
	bool GetIsHitPlayer() { return m_isHitBall; }

};
