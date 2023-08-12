/**********************************************
 *
 *  Spike.h
 *  Spike.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once
#include "Colliders/SphereCollider/SphereCollider.h"


class Spike
{
private:
	//	���a
	static const float RADIUS;
	//	����
	static const float HEIGHT;
	//	��]
	static const DirectX::SimpleMath::Vector3 ANGLE;

	//	�d��
	static const float GRAVITY;

	//	�R���C�_�[�̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3 COLLIDER_OFFSET;

private:
	//	�W�I���g���b�N�v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive>	m_geometricPrimitive;

	//	���W
	DirectX::SimpleMath::Vector3	m_pos;
	//	�����x
	DirectX::SimpleMath::Vector3	m_acce;

	//	�R���C�_�[
	std::unique_ptr<SphereCollider> m_collider;

	//	�X�p�C�N�̗L�����t���O
	bool							m_isEnable;

public:
	//	�R���X�g���N�^
	Spike(ID3D11DeviceContext* context);
	//	�f�X�g���N�^
	~Spike();

	//	����������
	void Init(const DirectX::SimpleMath::Vector3& pos);

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

public:
	//	���W�̎擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_pos; }
	//	�R���C�_�[�̎擾
	SphereCollider* GetCollider() const { return m_collider.get(); }
	//	�A�N�e�B�u�̎擾
	bool GetIsEnable() const { return m_isEnable; }

public:
	//	�A�N�e�B�u�̐ݒ�
	void SetIsEnable(const bool& enable) { m_isEnable = enable; }

};


