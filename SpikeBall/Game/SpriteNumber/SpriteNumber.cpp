/**********************************************
 *
 *  SpriteNumber.cpp
 *  スプライトによる数字の描画
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "SpriteNumber.h"
#include "Utilities/UtilityFunction.h"
#include "WICTextureLoader.h"

using namespace DirectX;

//	テクスチャパス
const wchar_t* const		SpriteNumber::TEXTURE_PATH = L"Resources/Textures/numberx128.png";
//	テクスチャの分割数
const SimpleMath::Vector2	SpriteNumber::TEXTURE_SPLIT_COUNT = SimpleMath::Vector2(11.0f, 1.0f);

//	数字の感覚
const SimpleMath::Vector2	SpriteNumber::NUMBER_GAP = SimpleMath::Vector2(96.0f, 0.0f);

//	アウトライン
const SimpleMath::Vector2	SpriteNumber::OUTLINE_OFFSET[8] =
{
	SimpleMath::Vector2(0, -1),		//	上
	SimpleMath::Vector2(1, -1),		//	右上
	SimpleMath::Vector2(0, 1),		//	右
	SimpleMath::Vector2(1, 1),		//	右下
	SimpleMath::Vector2(1, 0),		//	下
	SimpleMath::Vector2(-1, 1),		//	左下
	SimpleMath::Vector2(-1, 0),		//	左
	SimpleMath::Vector2(-1, -1),	//	左上
};


//	コンストラクタ
SpriteNumber::SpriteNumber(ID3D11Device* device, ID3D11DeviceContext* context):
	m_color(Colors::White),
	m_outlineColor(Colors::Black)
{
	//	スプライトバッチの作成
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	//	テクスチャの読み込み
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(
			device,
			TEXTURE_PATH,
			resource.ReleaseAndGetAddressOf(),
			m_texture.ReleaseAndGetAddressOf()
		)
	);

	//	テクスチャサイズの取得
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	resource.As(&texture);
	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);
	//	サイズを保持
	m_textureSize.x = static_cast<float>(desc.Width);
	m_textureSize.y = static_cast<float>(desc.Height);

	resource.Reset();
	texture.Reset();
}

//	デストラクタ
SpriteNumber::~SpriteNumber()
{
}


//--------------------------------------------------------------------------------
// 描画開始
//--------------------------------------------------------------------------------
void SpriteNumber::Begin(const CommonStates& states)
{
	//	描画開始
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
}

//--------------------------------------------------------------------------------
// 描画終了
//--------------------------------------------------------------------------------
void SpriteNumber::End()
{
	//	描画終了
	m_spriteBatch->End();
}

//--------------------------------------------------------------------------------
// 数値の描画
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumber(
	const int& num, 
	const SimpleMath::Vector2& pos, 
	const Anchor& anchor,
	const SimpleMath::Vector2& offset, 
	const int& outlineWidth, 
	const int& digits
)
{
	//	アウトラインの描画
	if (outlineWidth > 0)
	{
		for (size_t i = 0; i < 8; i++)
		{
			DrawNumberBase(num, pos + OUTLINE_OFFSET[i] * static_cast<float>(outlineWidth), anchor, offset, digits, m_outlineColor);
		}
	}

	//	数字の描画
	DrawNumberBase(num, pos, anchor, offset, digits, m_color);
}

//--------------------------------------------------------------------------------
// 数値の描画（RECT指定版）
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumber(
	const RECT& rect,
	const SimpleMath::Vector2& pos,
	const Anchor& anchor, 
	const SimpleMath::Vector2& offset, 
	const int& outlineWidth
)
{
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(m_textureSize.x / TEXTURE_SPLIT_COUNT.x, m_textureSize.y / TEXTURE_SPLIT_COUNT.y);

	//	文字位置を計算
	float anchorOffset = 0;
	if (anchor != Anchor::LEFT)
		anchorOffset -= (rect.right - rect.left) / (int)anchor;			//	アンカー座標をずらす

	DirectX::SimpleMath::Vector2 numPos = pos + NUMBER_GAP * anchorOffset;
	//	オフセットを加算
	numPos += offset;

	//	アウトラインの描画
	if (outlineWidth > 0)
	{
		for (size_t i = 0; i < 8; i++)
		{
			m_spriteBatch->Draw(m_texture.Get(), numPos + OUTLINE_OFFSET[i] * static_cast<float>(outlineWidth), &rect, m_outlineColor, 0.0f);
		}
	}

	//	描画
	m_spriteBatch->Draw(m_texture.Get(), numPos, &rect, m_color, 0.0f);
}

//--------------------------------------------------------------------------------
// 数値の描画
//--------------------------------------------------------------------------------
void SpriteNumber::DrawNumberBase(
	const int& num,
	const SimpleMath::Vector2& pos,
	const Anchor& anchor,
	const SimpleMath::Vector2& offset,
	const int& digits,
	const XMVECTORF32& color
)
{
	const SimpleMath::Vector2 TEXTURE_SPRITE_SIZE = SimpleMath::Vector2(m_textureSize.x / TEXTURE_SPLIT_COUNT.x, m_textureSize.y / TEXTURE_SPLIT_COUNT.y);

	//	桁数
	int dig = GetDigits(num);
	//	数値を文字列に変換
	std::string numStr = std::to_string(num);

	//	目標の桁数に満たないときは0埋めする
	while (dig < digits)
	{
		//	文字列の先頭に0を付け足す
		numStr = "0" + numStr;
		//	桁数を加算
		dig++;
	}

	//	数字の描画
	for (int i = 0; i < dig; i++)
	{
		int n = numStr[i] - '0';		//	一桁取り出す

		if (0 > n || n > 9)				//	数値が 0 ~ 9 の範囲外ならスキップ
			continue;

		RECT rect;						//	テクスチャの切り抜き位置
		rect.left	= static_cast<int>(TEXTURE_SPRITE_SIZE.x * n);				//	左
		rect.right	= static_cast<LONG>(TEXTURE_SPRITE_SIZE.x * (n + 1));		//	右
		rect.top	= 0;														//	上
		rect.bottom = static_cast<LONG>(TEXTURE_SPRITE_SIZE.y);					//	下

		//	文字位置を計算
		float anchorOffset = static_cast<float>(i);
		if (anchor != Anchor::LEFT)
			anchorOffset -= (float)dig / (int)anchor;			//	アンカー座標をずらす

		DirectX::SimpleMath::Vector2 numPos = pos + NUMBER_GAP * anchorOffset;
		//	オフセットを加算
		numPos += offset;

		//	描画
		m_spriteBatch->Draw(m_texture.Get(), numPos, &rect, color, 0.0f);
	}
}