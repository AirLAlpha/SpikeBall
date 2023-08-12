/**********************************************
 *
 *  Button.h
 *  Button.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once

#include <memory>
#include <functional>
#include <SimpleMath.h>
#include "2DShapeRender/ShapeRender2D.h"

class Button
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;		//	�e�N�X�`��
	std::unique_ptr<ShapeRender2D>						m_shapeRender;	//	�}�`�`��

	std::function<void(void)> m_pressedFunction;		//	�I�����ɌĂяo���֐�

	DirectX::SimpleMath::Vector2	m_pos;				//	���W
	DirectX::SimpleMath::Vector2	m_size;				//	�T�C�Y
	DirectX::XMVECTORF32			m_color;			//	�F

public:
	//	�R���X�g���N�^
	Button(
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const DirectX::SimpleMath::Vector2& pos,
		const DirectX::SimpleMath::Vector2& size,
		std::function<void(void)> func = NULL, 
		const wchar_t* texturePath = nullptr
	);
	//	�f�X�g���N�^
	~Button();

	void Update();		//	�X�V����
	void Render(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	�`�揈��

	void SetPosition(const DirectX::SimpleMath::Vector2& pos)	{ m_pos = pos; }		//	���W�̐ݒ�
	void SetSize(const DirectX::SimpleMath::Vector2& size)		{ m_size = size; }		//	�T�C�Y�̐ݒ�
};
