/**********************************************
 *
 *  Line.h
 *  �����I�u�W�F�N�g�̏������L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/29
 *
 **********************************************/
#pragma once

#include "SimpleMath.h"

class Line
{
private:
	DirectX::SimpleMath::Vector3 m_posA;		//	���WA
	DirectX::SimpleMath::Vector3 m_posB;		//	���WB

public:
	//	�R���X�g���N�^
	Line(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b) :
		m_posA(a), m_posB(b)
	{
	}

	//	�����̎擾
	float GetLength() const
	{
		return DirectX::SimpleMath::Vector3::Distance(m_posA, m_posB);
	}

	//	�x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetVector() const
	{
		return m_posB - m_posA;
	}
	//	���K�����ꂽ�x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetNormalizedVector() const
	{
		DirectX::SimpleMath::Vector3 ret = GetVector();
		ret.Normalize();
		return ret;
	}

	DirectX::SimpleMath::Vector3 GetPosA() const { return this->m_posA; }	//	���WA�̎擾
	DirectX::SimpleMath::Vector3 GetPosB() const { return this->m_posB; }	//	���WB�̎擾
};
