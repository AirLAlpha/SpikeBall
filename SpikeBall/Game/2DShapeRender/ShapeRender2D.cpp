/**********************************************
 *
 *  ShapeRender2D.cpp
 *  シンプルな2D図形を描画する処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "ShapeRender2D.h"
#include "Utilities/Screen.h"

using namespace DirectX;

ShapeRender2D::ShapeRender2D(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexType>>(context);
	//	ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);				//	頂点カラーの有効化
	m_basicEffect->SetTextureEnabled(false);					//	テクスチャの有効化

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexType>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	射影行列の作成（ピクセル単位にし、左上を基準とする）
	float width = static_cast<float>(Screen::WIDTH);
	float height = static_cast<float>(Screen::HEIGHT);
	SimpleMath::Matrix projection =
		SimpleMath::Matrix(
			SimpleMath::Vector4(2 / width, 0, 0, 0),
			SimpleMath::Vector4(0, -2 / height, 0, 0),
			SimpleMath::Vector4(0, 0, 1, 0),
			SimpleMath::Vector4(0, 0, 0, 1)
		)
		*
		SimpleMath::Matrix(
			SimpleMath::Vector4(1, 0, 0, 0),
			SimpleMath::Vector4(0, 1, 0, 0),
			SimpleMath::Vector4(0, 0, 1, 0),
			SimpleMath::Vector4(-1, 1, 0, 1)
		);

	//	各行列を設定
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(SimpleMath::Matrix::Identity);
	m_basicEffect->SetProjection(projection);
}

ShapeRender2D::~ShapeRender2D()
{
}


//--------------------------------------------------------------------------------
// 描画の開始
//--------------------------------------------------------------------------------
void ShapeRender2D::Begin(ID3D11DeviceContext* context, const CommonStates& states)
{
	//	カリングの設定
	context->RSSetState(states.CullNone());
	//	インプットレイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());
	//	ブレンドモードの設定
	context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xFFFFFF);

	//	テクスチャがループする用に設定
	auto sampler = states.LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);

	//	ベーシックエフェクトの適応
	m_basicEffect->Apply(context);

	//	描画開始
	m_primitiveBatch->Begin();
}

//--------------------------------------------------------------------------------
// 描画の終了
//--------------------------------------------------------------------------------
void ShapeRender2D::End()
{
	//	描画終了
	m_primitiveBatch->End();
}

//--------------------------------------------------------------------------------
// 三角形の描画
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawTriangle(Vector pos1, Vector pos2, Vector pos3, XMVECTORF32 color)
{
	const VertexType V[3] =
	{
		VertexType(pos1,color,SimpleMath::Vector2(0.5f, 0)),
		VertexType(pos2,color,SimpleMath::Vector2(0,1)),
		VertexType(pos3,color,SimpleMath::Vector2(1,1)),
	};

	m_primitiveBatch->DrawTriangle(V[0], V[1], V[2]);
}

//--------------------------------------------------------------------------------
// 線の描画
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawLine(Vector pos1, Vector pos2, XMVECTORF32  color)
{
	const VertexType V[2] =
	{
		VertexType(pos1, color,SimpleMath::Vector2(0,0)),
		VertexType(pos2, color,SimpleMath::Vector2(1,1)),
	};

	m_primitiveBatch->DrawLine(V[0], V[1]);
}

//--------------------------------------------------------------------------------
// 矩形の描画処理
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawBox(Vector pos1, Vector pos2, XMVECTORF32  color, SimpleMath::Vector2 uvScale)
{
	SimpleMath::Vector3 p1 = pos1;
	SimpleMath::Vector3 p2 = pos2;

	const VertexType V[4] =
	{
		VertexType(p1,									color,	SimpleMath::Vector2(0,0)),
		VertexType(SimpleMath::Vector3(p2.x,p1.y,p2.z),	color,	SimpleMath::Vector2(uvScale.x,0)),
		VertexType(p2,									color,	SimpleMath::Vector2(uvScale.x,uvScale.y)),
		VertexType(SimpleMath::Vector3(p1.x,p2.y,p1.z),	color,	SimpleMath::Vector2(0,uvScale.y)),
	};

	m_primitiveBatch->DrawQuad(V[0], V[1], V[2], V[3]);
}

//--------------------------------------------------------------------------------
// 円の描画処理
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawCircle(Vector centerPos, float radius, XMVECTORF32  color)
{
	SimpleMath::Vector2 pos;
	XMStoreFloat2(&pos, centerPos);					//	XMVECTORをVector2に変換

	const int DIV					= 64;			//	分割数
	const int VERTEX_COUNT			= DIV + 1;		//	頂点数（円周 + 中心）
	const int POLYGON_VERT_COUNT	= 3;			//	1ポリゴンに対する頂点数

	VertexType		verticies[VERTEX_COUNT];			//	頂点バッファ
	uint16_t		indicies[DIV * POLYGON_VERT_COUNT];	//	インデックスバッファ

	//	中心の頂点を設定		※XMStoreFloat3関数は 引数1に 引数2をコピーする
	XMStoreFloat3(&verticies[0].position,	centerPos);			//	座標
	XMStoreFloat4(&verticies[0].color,		color);				//	色

	//	円周上の頂点を設定
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		float x = pos.x + cosf(XM_2PI / DIV * i) * radius;		//	X座標
		float y = pos.y + sinf(XM_2PI / DIV * i) * radius;		//	Y座標

		verticies[i].position = SimpleMath::Vector3(x, y, 0.0f);			//	座標の設定
		XMStoreFloat4(&verticies[i].color, color);				//	色
	}

	//	インデックスを指定
	for (int i = 0; i < DIV; i++)
	{
		indicies[i * POLYGON_VERT_COUNT + 0] = 0;
		indicies[i * POLYGON_VERT_COUNT + 1] = static_cast<uint16_t>(i + 1);
		indicies[i * POLYGON_VERT_COUNT + 2] = static_cast<uint16_t>(i + 2);
	}
	indicies[DIV * POLYGON_VERT_COUNT - 1] = indicies[1];		//	最後の頂点は一周して重なる1番を設定する（0は中心）

	//	描画
	m_primitiveBatch->DrawIndexed(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indicies,
		DIV * POLYGON_VERT_COUNT,
		verticies,
		VERTEX_COUNT
	);
}

//--------------------------------------------------------------------------------
// テクスチャの使用を切り替え
//--------------------------------------------------------------------------------
void ShapeRender2D::SetTextureEnable(ID3D11Device* device, bool enableTexture)
{
	m_basicEffect->SetTextureEnabled(enableTexture);
	
	//	インプットレイアウトの再生成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexType>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

//--------------------------------------------------------------------------------
// テクスチャの設定
//--------------------------------------------------------------------------------
void ShapeRender2D::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_basicEffect->SetTexture(texture);
}
