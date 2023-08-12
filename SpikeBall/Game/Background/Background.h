/**********************************************
 *
 *  Background.h
 *  Background.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once

class Background
{
private:
	//	�T�C�Y
	static const float SIZE;
	//	��]���x
	static const float SPEED;

private:
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		m_inputLayout;
	//	�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect>			m_basicEffect;

	//	�W�I���g���b�N�v���~�e�B�u
	std::unique_ptr<DirectX::GeometricPrimitive>	m_geometricPrimitive;
	//	��]��
	float											m_angle;

public:
	Background(ID3D11Device* device, ID3D11DeviceContext* context);
	~Background();

	//	�X�V����
	void Update(const float& deltaTime);
	//	�`�揈��
	void Render(
		ID3D11DeviceContext* context, 
		const DirectX::SimpleMath::Matrix& view, 
		const DirectX::SimpleMath::Matrix& proj
	);


};