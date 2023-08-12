/**********************************************
 *
 *  SpriteNumber.h
 *  SpriteNumber.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#pragma once


class SpriteNumber
{
public:
	//	アンカー
	enum class Anchor
	{
		LEFT	= 0,	//	左
		RIGHT	= 1,	//	右
		CENTER	= 2,	//	中央
	};

public:
	//	テクスチャパス
	static const wchar_t* const		TEXTURE_PATH;

	//	テクスチャの分割数
	static const DirectX::SimpleMath::Vector2	TEXTURE_SPLIT_COUNT;
	//	数字の感覚
	static const DirectX::SimpleMath::Vector2	NUMBER_GAP;
	//	アウトラインのオフセット
	static const DirectX::SimpleMath::Vector2	OUTLINE_OFFSET[8];


private:
	//	スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;

	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;
	//	テクスチャサイズ
	DirectX::SimpleMath::Vector2						m_textureSize;

	//	色
	DirectX::XMVECTORF32	m_color;
	//	アウトライン色
	DirectX::XMVECTORF32	m_outlineColor;

private:
	//	数値の描画
	void DrawNumberBase(
		const int& num,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& digits = -1,
		const DirectX::XMVECTORF32& color = DirectX::Colors::White
	);


public:
	SpriteNumber(ID3D11Device* device, ID3D11DeviceContext* context);
	~SpriteNumber();

	//	描画開始
	void Begin(const DirectX::CommonStates& states);
	//	描画終了
	void End();

	//	数値の描画
	void DrawNumber(
		const int& num,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& outlineWidth = 0,
		const int& digits = -1
	);

	//	数値の描画（RECT指定版）
	void DrawNumber(
		const RECT& rect,
		const DirectX::SimpleMath::Vector2& pos,
		const Anchor& anchor,
		const DirectX::SimpleMath::Vector2& offset = DirectX::SimpleMath::Vector2::Zero,
		const int& outlineWidth = 0
	);


public:
	//	色の設定
	void SetColor(const DirectX::XMVECTORF32& color) { m_color = color; }
	//	アウトライン色の設定
	void SetOutlineColor(const DirectX::XMVECTORF32& color) { m_outlineColor = color; }

	//	テクスチャサイズの取得
	DirectX::SimpleMath::Vector2 GetTextureSize() { return m_textureSize; }

};
