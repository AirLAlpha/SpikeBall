/**********************************************
 *
 *  Line.h
 *  線分オブジェクトの処理を記述
 *
 *  製作者：牛丸文仁
 *  制作日：2023/03/29
 *
 **********************************************/
#pragma once

#include "SimpleMath.h"

class Line
{
private:
	DirectX::SimpleMath::Vector3 m_posA;		//	座標A
	DirectX::SimpleMath::Vector3 m_posB;		//	座標B

public:
	//	コンストラクタ
	Line(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b) :
		m_posA(a), m_posB(b)
	{
	}

	//	長さの取得
	float GetLength() const
	{
		return DirectX::SimpleMath::Vector3::Distance(m_posA, m_posB);
	}

	//	ベクトルの取得
	DirectX::SimpleMath::Vector3 GetVector() const
	{
		return m_posB - m_posA;
	}
	//	正規化されたベクトルの取得
	DirectX::SimpleMath::Vector3 GetNormalizedVector() const
	{
		DirectX::SimpleMath::Vector3 ret = GetVector();
		ret.Normalize();
		return ret;
	}

	DirectX::SimpleMath::Vector3 GetPosA() const { return this->m_posA; }	//	座標Aの取得
	DirectX::SimpleMath::Vector3 GetPosB() const { return this->m_posB; }	//	座標Bの取得
};
