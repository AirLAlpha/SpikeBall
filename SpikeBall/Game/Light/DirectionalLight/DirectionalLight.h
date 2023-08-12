/**********************************************
 *
 *  DirectionalLight.h
 *  DirectionalLight.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/07/03
 *
 **********************************************/
#pragma once

#if _DEBUG
#include "ImaseLib/DebugFont/DebugFont.h"
#endif

class DirectionalLight
{
private:
	static const float DEFAULT_DISTANCE;		//	�f�t�H���g�̋���
	static const float DEFAULT_SIZE;			//	�f�t�H���g�̃T�C�Y�i�͈́j
	static const float DEFAULT_NEAR;			//	�f�t�H���g�̃j�A�N���b�v
	static const float DEFAULT_FAR;				//	�f�t�H���g�̃t�@�[�N���b�v

private:
	//	����
	DirectX::SimpleMath::Vector2	m_angle;
	//	��]
	DirectX::SimpleMath::Quaternion m_rot;
	//	����
	float							m_dist;
	//	�傫���i�͈́j
	float							m_size;


#if _DEBUG
	std::unique_ptr<Imase::DebugFont> m_debugFont;
#endif

public:
	//	�R���X�g���N�^
	DirectionalLight(const DirectX::SimpleMath::Vector2& dir);
	//	�f�X�g���N�^
	~DirectionalLight();

#if _DEBUG
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	void DebugRender(DirectX::CommonStates* commonStates);
#endif

public:
	//	�����̎擾
	DirectX::SimpleMath::Vector3 GetDirection() const;

	//	�r���[�s��̎擾
	DirectX::SimpleMath::Matrix GetLightView() const;
	//	�ˉe�s��̎擾
	DirectX::SimpleMath::Matrix GetLightProjection() const;

public:
	//	�����̐ݒ�
	void SetAngle(const DirectX::SimpleMath::Vector2& angle);


};
