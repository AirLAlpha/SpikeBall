/**********************************************
 *
 *  Ball.h
 *  Ball.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/27
 *
 **********************************************/
#pragma once
#include "Colliders/SphereCollider/SphereCollider.h"

//	�O���錾
class Floor;


class Ball
{
private:
	//	�d��
	static const float GRAVITY;
	//	���x�����p
	static const float SPEED_MOD;

private:
	//	���f��
	std::unique_ptr<DirectX::Model>		m_model;
	//	�R���C�_�[
	std::unique_ptr<SphereCollider>		m_collider;


private:
	//	���W
	DirectX::SimpleMath::Vector3		m_pos;
	//	�����x
	DirectX::SimpleMath::Vector3		m_acce;
	//	���a
	float								m_radius;
	//	��]
	DirectX::SimpleMath::Quaternion		m_rot;

	//	�ݒu�t���O
	bool								m_onGround;

	//	���̖@��
	DirectX::SimpleMath::Vector3		m_groundNormal;
	//	���̉�]��
	DirectX::SimpleMath::Vector2		m_groundAngle;

private:
	//	�ړ�����
	void MoveUpdate(const float& deltaTime);

public:
	//	�R���X�g���N�^
	Ball(
		ID3D11Device* device,
		const DirectX::SimpleMath::Vector3& pos,
		const float radius
	);
	//	�f�X�g���N�^
	~Ball();

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(
		ID3D11DeviceContext* context,
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

	//	���Ƃ̏Փ˔���
	void CheckHitFloor(const Floor& floor);

public:
	//	���W�̐ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; m_collider->SetPosition(pos); }

public:
	//	���W�̎擾
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_pos; }
	//	���a�̎擾
	float GetRadius() const { return m_radius; }
	//	�R���C�_�[�̎擾
	SphereCollider* GetCollider() const { return m_collider.get(); }

};

