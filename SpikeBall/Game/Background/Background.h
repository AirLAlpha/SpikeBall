/**********************************************
 *
 *  Background.h
 *  Background.cppのヘッダーファイル
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/30
 *
 **********************************************/
#pragma once

class Background
{
private:
	//	サイズ
	static const float SIZE;
	//	回転速度
	static const float SPEED;

private:
	//	インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputLayout;
	//	ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect>			m_basicEffect;

	//	ジオメトリックプリミティブ
	std::unique_ptr<DirectX::GeometricPrimitive>	m_geometricPrimitive;
	//	回転量
	float											m_angle;

public:
	Background(ID3D11Device* device, ID3D11DeviceContext* context);
	~Background();

	//	更新処理
	void Update(const float& deltaTime);
	//	描画処理
	void Render(
		ID3D11DeviceContext* context, 
		const DirectX::SimpleMath::Matrix& view, 
		const DirectX::SimpleMath::Matrix& proj
	);


};