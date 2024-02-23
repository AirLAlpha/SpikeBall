/**********************************************
 *
 *  Floor.h
 *  Floor.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/27
 *
 **********************************************/
#pragma once
#include "Colliders/PlaneCollider/PlaneCollider.h"
#include "ShadowMap/ShadowMap.h"
#include "Effects/ShadowReceiveEffect/ShadowReceiveEffect.h"

class Floor
{
private:
	//	���f���̃X�P�[��
	static const DirectX::SimpleMath::Vector3	MODEL_SCALE;
	//	���f���̃I�t�Z�b�g
	static const DirectX::SimpleMath::Vector3	MODEL_OFFSET;
	//	�R���C�_�[�̃X�P�[��
	static const DirectX::SimpleMath::Vector2	COLLIDER_SCALE;
	//	��]���x
	static const float							ROTATE_SPEED;

	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�e�G�t�F�N�g
	std::unique_ptr<ShadowReceiveEffect> m_shadowReceiveEffect;
	//	���C�g
	DirectionalLight* m_light;

private:
	//	���f��
	std::unique_ptr<DirectX::Model>		m_model;
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//	�R���C�_�[
	std::unique_ptr<PlaneCollider>		m_collider;

private:
	//	��]��
	DirectX::SimpleMath::Vector2		m_angle;
	//	��]
	DirectX::SimpleMath::Quaternion		m_rot;

private:
	//	��]����
	void RotateUpdate(const float& deltaTime);

public:
	//	�R���X�g���N�^
	Floor(ID3D11Device* device);
	//	�f�X�g���N�^
	~Floor();

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(
		ID3D11DeviceContext* context, 
		const DirectX::CommonStates& states, 
		const DirectX::SimpleMath::Matrix& view, 
		const DirectX::SimpleMath::Matrix& proj
	);

	//	�V���h�E�}�b�v�ւ̕`��
	void ShadowMapRender(
		const ShadowMap& shadowMap,
		ID3D11DeviceContext* context,
		const DirectX::CommonStates& states
	);

public:
	//	�R���C�_�[�̎擾
	PlaneCollider* GetCollider() const { return m_collider.get(); }
	//	��]�ʂ̎擾
	DirectX::SimpleMath::Vector2 GetAngle() const { return m_angle; }

	//	���C�g�̐ݒ�
	void SetLight(DirectionalLight* light) { m_light = light; }

};
