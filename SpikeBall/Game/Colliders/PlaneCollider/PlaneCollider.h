/**********************************************
 *
 *  PlaneCollider.h
 *  PlaneCollider.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/29
 *
 **********************************************/
#pragma once

//	�O���錾
class SphereCollider;
class Line;


class PlaneCollider
{
#if _DEBUG
private:
	//	���_�̐�
	static const int VERTEX_COUNT = 4;
	//	���_
	static const DirectX::SimpleMath::Vector3 VERTICIES[VERTEX_COUNT];

private:
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;
	//	�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;
	
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	m_primitiveBatch;
#endif

private:
	//	��������
	DirectX::SimpleMath::Plane		m_plane;

	//	���W
	DirectX::SimpleMath::Vector3	m_pos;
	//	�T�C�Y
	DirectX::SimpleMath::Vector2	m_size;
	//	��]
	DirectX::SimpleMath::Quaternion m_rot;
	//	�@��
	DirectX::SimpleMath::Vector3	m_normal;


public:
	//	�R���X�g���N�^
	PlaneCollider(
		const DirectX::SimpleMath::Vector3& pos,
		const DirectX::SimpleMath::Vector2& size,
		const DirectX::SimpleMath::Quaternion& rot,
		const DirectX::SimpleMath::Vector3& normal = DirectX::SimpleMath::Vector3::Up
	);
	//	�f�X�g���N�^
	~PlaneCollider();

public:
	//	���W�̐ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos)	{ m_pos = pos; m_plane = DirectX::SimpleMath::Plane(pos, m_normal); }
	//	�T�C�Y�̐ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size)		{ m_size = size; }
	//	��]�̐ݒ�
	void SetRotate(const DirectX::SimpleMath::Quaternion& rot)	
	{ 
		m_rot = rot; 

		//	�@������]������
		m_normal = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, rot);
		m_plane = DirectX::SimpleMath::Plane(m_pos, m_normal);
	}

	//	�X�t�B�A�R���C�_�[�Ƃ̏Փ˔���
	bool CheckHit(const SphereCollider& other, DirectX::SimpleMath::Vector3* hitPoint = nullptr) const;
	//	�����Ƃ̏Փ˔���
	bool CheckHit(const Line& line, DirectX::SimpleMath::Vector3* hitPoint = nullptr) const;


public:
	//	�������ʂ̎擾
	DirectX::SimpleMath::Plane GetPlane() const { return m_plane; }
	//	�@���x�N�g���̎擾
	DirectX::SimpleMath::Vector3 GetNormal() const { return m_normal; }

#if _DEBUG
public:
	//	�f�o�b�O�p����������
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	//	�f�o�b�O�p�\��
	void DebugRender(
		ID3D11DeviceContext* context, 
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);
#endif
};

