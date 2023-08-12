/**********************************************
 *
 *  SphereCollider.h
 *  SphereCollider.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/28
 *
 **********************************************/
#pragma once

//	�O���錾
class PlaneCollider;
class Line;

class SphereCollider
{
#if _DEBUG
private:
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;
	//	�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>	m_primitiveBatch;
#endif
private:
	//	���W
	DirectX::SimpleMath::Vector3	m_pos;
	//	���a
	float							m_radius;


public:
	//	�R���X�g���N�^
	SphereCollider(const DirectX::SimpleMath::Vector3& pos, const float& radius);
	//	�f�X�g���N�^
	~SphereCollider();

	//	�������ʂƂ̏Փ˔���
	bool CheckHit(const DirectX::SimpleMath::Plane& plane) const;
	//	�����Ƃ̏Փ˔���
	bool CheckHit(const Line& line) const;
	//	�X�t�B�A�R���C�_�[���m�̏Փ˔���
	bool CheckHit(const SphereCollider& other) const;


public:
	//	���W�̐ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_pos = pos; }

public:
	//	���W�̎擾
	DirectX::SimpleMath::Vector3 GetPosition()	const { return m_pos; }
	//	���a�̎擾
	float GetRadius()					const {return m_radius;}

#if _DEBUG
public:
	//	�f�o�b�O�p�̏���������
	void DebugInit(ID3D11Device* device, ID3D11DeviceContext* context);
	//	�f�o�b�O�p�̕`�揈��
	void DebugRender(
		ID3D11DeviceContext* context,
		const DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj
	);

#endif
};
