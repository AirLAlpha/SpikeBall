/**********************************************
 *
 *  ShapeRender2D.h
 *  ShapeRender2D.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#pragma once

class ShapeRender2D
{
private:
	using VertexType	= DirectX::VertexPositionColorTexture;		//	頂点タイプ
	using Vector		= DirectX::XMVECTOR;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;		//	インプットレイアウト
	std::unique_ptr<DirectX::PrimitiveBatch<VertexType>>	m_primitiveBatch;	//	プリミティブバッチ
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;		//	ベーシックエフェクト

public:
	//	コンストラクタ
	ShapeRender2D(ID3D11Device* device, ID3D11DeviceContext* context);
	//	デストラクタ
	~ShapeRender2D();

	void Begin(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	描画開始
	void End();			//	描画終了

	//	三角形の描画処理
	void DrawTriangle(Vector pos1, Vector pos2, Vector pos3, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	//	線の描画処理
	void DrawLine(Vector pos1, Vector pos2, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	//	矩形の描画処理
	void DrawBox(Vector pos1, Vector pos2, DirectX::XMVECTORF32 color = DirectX::Colors::White, DirectX::SimpleMath::Vector2 uvScale = DirectX::SimpleMath::Vector2::One);
	//	円の描画処理
	void DrawCircle(Vector centerPos, float radius, DirectX::XMVECTORF32 color = DirectX::Colors::White);

	//	テクスチャの切り替え
	void SetTextureEnable(ID3D11Device* device, bool enableTexture);
	//	テクスチャの設定
	void SetTexture(ID3D11ShaderResourceView* texture);
};

