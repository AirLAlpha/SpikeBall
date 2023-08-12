/**********************************************
 *
 *  Background.cpp
 *  背景
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/31
 *
 **********************************************/
#include "pch.h"
#include "Background.h"
#include "Utilities/UtilityFunction.h"

using namespace DirectX;

//	サイズ
const float Background::SIZE = 100.0f;
//	スピード
const float Background::SPEED= 0.1f;


//	コンストラクタ
Background::Background(ID3D11Device* device, ID3D11DeviceContext* context):
	m_angle(0.0f)
{
	//	ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);

	//	インプットレイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect< VertexPositionNormalTexture>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	ジオメトリックプリミティブの作成
	m_geometricPrimitive = GeometricPrimitive::CreateSphere(context, SIZE);

}

//	デストラクタ
Background::~Background()
{
}

//--------------------------------------------------------------------------------
// 更新処理
//--------------------------------------------------------------------------------
void Background::Update(const float& deltaTime)
{
	m_angle += deltaTime * SPEED;
	m_angle = Repeat(m_angle, XM_2PI);
}

//--------------------------------------------------------------------------------
// 描画処理
//--------------------------------------------------------------------------------
void Background::Render(
	ID3D11DeviceContext* context, 
	const DirectX::SimpleMath::Matrix& view, 
	const DirectX::SimpleMath::Matrix& proj
)
{
	//	ワールド行列の作成
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateRotationY(m_angle);

	//	行列の設定
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);

	//	描画
	m_geometricPrimitive->Draw(m_basicEffect.get(), m_inputLayout.Get(), false, true);
}
