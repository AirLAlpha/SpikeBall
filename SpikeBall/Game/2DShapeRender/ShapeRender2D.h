/**********************************************
 *
 *  ShapeRender2D.h
 *  ShapeRender2D.cpp�̃w�b�_�[�t�@�C��
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#pragma once

class ShapeRender2D
{
private:
	using VertexType	= DirectX::VertexPositionColorTexture;		//	���_�^�C�v
	using Vector		= DirectX::XMVECTOR;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout>				m_inputLayout;		//	�C���v�b�g���C�A�E�g
	std::unique_ptr<DirectX::PrimitiveBatch<VertexType>>	m_primitiveBatch;	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::BasicEffect>					m_basicEffect;		//	�x�[�V�b�N�G�t�F�N�g

public:
	//	�R���X�g���N�^
	ShapeRender2D(ID3D11Device* device, ID3D11DeviceContext* context);
	//	�f�X�g���N�^
	~ShapeRender2D();

	void Begin(ID3D11DeviceContext* context, const DirectX::CommonStates& states);		//	�`��J�n
	void End();			//	�`��I��

	//	�O�p�`�̕`�揈��
	void DrawTriangle(Vector pos1, Vector pos2, Vector pos3, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	//	���̕`�揈��
	void DrawLine(Vector pos1, Vector pos2, DirectX::XMVECTORF32 color = DirectX::Colors::White);
	//	��`�̕`�揈��
	void DrawBox(Vector pos1, Vector pos2, DirectX::XMVECTORF32 color = DirectX::Colors::White, DirectX::SimpleMath::Vector2 uvScale = DirectX::SimpleMath::Vector2::One);
	//	�~�̕`�揈��
	void DrawCircle(Vector centerPos, float radius, DirectX::XMVECTORF32 color = DirectX::Colors::White);

	//	�e�N�X�`���̐؂�ւ�
	void SetTextureEnable(ID3D11Device* device, bool enableTexture);
	//	�e�N�X�`���̐ݒ�
	void SetTexture(ID3D11ShaderResourceView* texture);
};

