/**********************************************
 *
 *  Timer.cpp
 *  タイマーUI
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "Timer.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	時間の最大値
const float Timer::TIME_MAX = (60.0f * 59 + 59 + 0.999f);
//	アウトラインの幅
const int	Timer::OUTLINE_WIDTH = 3;


//	コンストラクタ
Timer::Timer(ID3D11Device* device, ID3D11DeviceContext* context, const SimpleMath::Vector2& pos):
	SpriteNumber(device, context),
	m_pos(pos),
	m_time(0.0f),
	m_isStop(false)
{
}

//	デストラクタ
Timer::~Timer()
{
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Timer::Update(const float& deltaTime)
{
	//	停止中は処理しない
	if (m_isStop)
		return;

	//	時間を加算する
	m_time += deltaTime;
	//	数値がオーバーフローしないようにクランプする
	m_time = Clump(m_time, 0.0f, TIME_MAX);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Timer::Render(const DirectX::CommonStates& states)
{
	SimpleMath::Vector2 textureSize = GetTextureSize();
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(textureSize.x / TEXTURE_SPLIT_COUNT.x, textureSize.y / TEXTURE_SPLIT_COUNT.y);

	int sec = static_cast<int>(m_time) % 60;					//	秒
	int min = static_cast<int>(m_time / 60);					//	分
	int ms	= static_cast<int>((m_time - (int)m_time) * 100);	//	ミリ秒

	RECT colonRect;							//	コロン用のRECTを作成
	colonRect.left		= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * 10);
	colonRect.right		= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * 11);
	colonRect.top		= 0;
	colonRect.bottom	= static_cast<LONG>(TEXTURE_SPRITE_SIZE.y);

	auto offset = [](int dig) { return NUMBER_GAP * static_cast<float>(dig); };		//	オフセットの計算をするラムダ式

	Begin(states);		//	描画開始

	//	分の描画
	DrawNumber(min, m_pos, Anchor::LEFT, SimpleMath::Vector2::Zero, OUTLINE_WIDTH, 2);
	//	コロンの描画
	SimpleMath::Vector2 colonOffset = offset(2);
	DrawNumber(colonRect, m_pos, Anchor::LEFT, colonOffset, OUTLINE_WIDTH);
	//	秒の描画
	SimpleMath::Vector2 secOffset = offset(3);
	DrawNumber(sec, m_pos, Anchor::LEFT, secOffset, OUTLINE_WIDTH, 2);
	//	コロンの描画
	colonOffset = offset(5);
	DrawNumber(colonRect, m_pos, Anchor::LEFT, colonOffset, OUTLINE_WIDTH);
	//	ミリ秒の描画
	SimpleMath::Vector2 msOffset = offset(6);
	DrawNumber(ms, m_pos, Anchor::LEFT, msOffset, OUTLINE_WIDTH, 2);

	End();			//	描画終了
}

//--------------------------------------------------------------------------------
// リセット
//--------------------------------------------------------------------------------
void Timer::Reset()
{
	//	時間をリセット
	m_time = 0.0f;
}
