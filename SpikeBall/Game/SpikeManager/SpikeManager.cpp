/**********************************************
 *
 *  SpikeManager.cpp
 *  スパイクの管理を行う
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "SpikeManager.h"
#include "Floor/Floor.h"
#include "Ball/Ball.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

//	スパイクの開始Y座標
const float SpikeManager::SPIKE_START_POS_Y = 15.0f;
//	スパイクが削除される高さ
const float SpikeManager::SPIKE_DELETE_POS_Y = -30.0f;
//	スパイクの生成間隔
const float SpikeManager::SPIKE_GENERATE_INTERVAL = 0.35f;



//	コンストラクタ
SpikeManager::SpikeManager(ID3D11DeviceContext* context):
	m_generateInterval(0.0f),
	m_isHitBall(false)
{
	//	リストの初期化
	for (int i = 0; i < SPIKE_COUNT; i++)
	{
		std::unique_ptr<Spike> newSpike = std::make_unique<Spike>(context);
		m_spikeList.emplace_back(std::move(newSpike));
	}
}

//	デストラクタ
SpikeManager::~SpikeManager()
{
}


//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void SpikeManager::Update(const float& deltaTime)
{
	GenerateUpdate(deltaTime);

	//	スパイクの更新処理
	for (const auto& spike : m_spikeList)
	{
		if (!spike->GetIsEnable())
			continue;

		spike->Update(deltaTime);

		//	削除する高さに到達したら非アクティブに設定する
		if (spike->GetPosition().y < SPIKE_DELETE_POS_Y)
			spike->SetIsEnable(false);
	}
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void SpikeManager::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	for (const auto& spike : m_spikeList)
	{
		if (!spike->GetIsEnable())
			continue;

		spike->Render(view, proj);
	}
}

//--------------------------------------------------------------------------------
// 床との衝突判定
//--------------------------------------------------------------------------------
void SpikeManager::CheckHitFloor(const Floor& floor)
{
	for (const auto& spike : m_spikeList)
	{
		//	アクティブでないスパイクはスキップ
		if (!spike->GetIsEnable())
			continue;

		//	衝突していたらスパイクを消す
		if (floor.GetCollider()->CheckHit(*spike->GetCollider()))
		{
			spike->SetIsEnable(false);
		}
	}
}

//--------------------------------------------------------------------------------
// ボールとの衝突判定
//--------------------------------------------------------------------------------
void SpikeManager::CheckHitBall(const Ball& ball)
{
	for (const auto& spike : m_spikeList)
	{
		//	アクティブでないスパイクはスキップ
		if (!spike->GetIsEnable())
			continue;

		//	衝突していたらフラグを立てる
		if (ball.GetCollider()->CheckHit(*spike->GetCollider()))
		{
			m_isHitBall = true;
		}
	}
}


//--------------------------------------------------------------------------------
// スパイクのランダム生成
//--------------------------------------------------------------------------------
void SpikeManager::GenerateSpike(const DirectX::SimpleMath::Vector3& range)
{
	//	未使用のスパイクを検索
	int index = -1;
	for (int i = 0; i < m_spikeList.size(); i++)
	{
		if (!m_spikeList[i]->GetIsEnable())
		{
			index = i;
			break;
		}
	}

	//	見つからなかったら処理をしない
	if (index == -1)
		return;

	//	大きさ1のランダムなベクトルを取得
	SimpleMath::Vector3 randPos = GetRandomVec();
	//	大きさを変更
	randPos.x *= range.x;
	randPos.y = SPIKE_START_POS_Y;
	randPos.z *= range.z;

	m_spikeList[index]->Init(randPos);
}

//--------------------------------------------------------------------------------
// スパイクの生成更新処理
//--------------------------------------------------------------------------------
void SpikeManager::GenerateUpdate(const float& deltaTime)
{
	m_generateInterval += deltaTime;
	if (m_generateInterval >= SPIKE_GENERATE_INTERVAL)
	{
		GenerateSpike(SimpleMath::Vector3::One * 9.0f);
		m_generateInterval = 0.0f;
	}
}

