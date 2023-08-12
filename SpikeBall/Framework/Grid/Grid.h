/**********************************************
 *
 *  Grid.h
 *  Grid.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/06/12
 *
 **********************************************/
#pragma once

class Grid
{
private:
	//	使用する頂点タイプ
	using VertexType = DirectX::VertexPositionColor;

	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;
	//	ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;
	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<VertexType>>	m_primitiveBatch;

	//	グリッドの大きさ
	DirectX::SimpleMath::Vector2							m_size;

public:
	Grid(ID3D11Device* device, ID3D11DeviceContext* context, const DirectX::SimpleMath::Vector2& size);
	~Grid() = default;

	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);

};
