/**********************************************
 *
 *  Grid.cpp
 *  グリッドの描画クラス
 *
 *  製作者：牛丸文仁
 *  制作日：2023/06/12
 *
 **********************************************/
#include "pch.h"
#include "Grid.h"

#include "UserResources.h"
#include "DebugDraw/DebugDraw.h"

using namespace DirectX;

//	コンストラクタ
Grid::Grid(ID3D11Device* device, ID3D11DeviceContext* context, const DirectX::SimpleMath::Vector2& size) :
	m_size(size)
{
	//	ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	//	プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexType>>(context);

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexType>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}


//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Grid::Render(const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj)
{
	//	必要なリソースを取得
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	行列の作成
	SimpleMath::Matrix world;
	world = SimpleMath::Matrix::CreateScale(SimpleMath::Vector3(m_size.x / 2.0f, 0.0f, m_size.y / 2.0f));

	//	行列の設定
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	//	インプットレイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	//	グリッド線の描画
	m_primitiveBatch->Begin();

	DX::DrawGrid(
		m_primitiveBatch.get(),
		SimpleMath::Vector3::Right, 
		SimpleMath::Vector3::Forward, 
		SimpleMath::Vector3::Zero, 
		static_cast<size_t>(m_size.x),
		static_cast<size_t>(m_size.y)
	);

	m_primitiveBatch->End();
}