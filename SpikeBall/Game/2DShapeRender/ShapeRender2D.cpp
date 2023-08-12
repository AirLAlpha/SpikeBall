/**********************************************
 *
 *  ShapeRender2D.cpp
 *  �V���v����2D�}�`��`�悷�鏈�����L�q
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/30
 *
 **********************************************/
#include "pch.h"
#include "ShapeRender2D.h"
#include "Utilities/Screen.h"

using namespace DirectX;

ShapeRender2D::ShapeRender2D(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	�v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexType>>(context);
	//	�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);				//	���_�J���[�̗L����
	m_basicEffect->SetTextureEnabled(false);					//	�e�N�X�`���̗L����

	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexType>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	�ˉe�s��̍쐬�i�s�N�Z���P�ʂɂ��A�������Ƃ���j
	float width = static_cast<float>(Screen::WIDTH);
	float height = static_cast<float>(Screen::HEIGHT);
	SimpleMath::Matrix projection =
		SimpleMath::Matrix(
			SimpleMath::Vector4(2 / width, 0, 0, 0),
			SimpleMath::Vector4(0, -2 / height, 0, 0),
			SimpleMath::Vector4(0, 0, 1, 0),
			SimpleMath::Vector4(0, 0, 0, 1)
		)
		*
		SimpleMath::Matrix(
			SimpleMath::Vector4(1, 0, 0, 0),
			SimpleMath::Vector4(0, 1, 0, 0),
			SimpleMath::Vector4(0, 0, 1, 0),
			SimpleMath::Vector4(-1, 1, 0, 1)
		);

	//	�e�s���ݒ�
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(SimpleMath::Matrix::Identity);
	m_basicEffect->SetProjection(projection);
}

ShapeRender2D::~ShapeRender2D()
{
}


//--------------------------------------------------------------------------------
// �`��̊J�n
//--------------------------------------------------------------------------------
void ShapeRender2D::Begin(ID3D11DeviceContext* context, const CommonStates& states)
{
	//	�J�����O�̐ݒ�
	context->RSSetState(states.CullNone());
	//	�C���v�b�g���C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());
	//	�u�����h���[�h�̐ݒ�
	context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xFFFFFF);

	//	�e�N�X�`�������[�v����p�ɐݒ�
	auto sampler = states.LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);

	//	�x�[�V�b�N�G�t�F�N�g�̓K��
	m_basicEffect->Apply(context);

	//	�`��J�n
	m_primitiveBatch->Begin();
}

//--------------------------------------------------------------------------------
// �`��̏I��
//--------------------------------------------------------------------------------
void ShapeRender2D::End()
{
	//	�`��I��
	m_primitiveBatch->End();
}

//--------------------------------------------------------------------------------
// �O�p�`�̕`��
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawTriangle(Vector pos1, Vector pos2, Vector pos3, XMVECTORF32 color)
{
	const VertexType V[3] =
	{
		VertexType(pos1,color,SimpleMath::Vector2(0.5f, 0)),
		VertexType(pos2,color,SimpleMath::Vector2(0,1)),
		VertexType(pos3,color,SimpleMath::Vector2(1,1)),
	};

	m_primitiveBatch->DrawTriangle(V[0], V[1], V[2]);
}

//--------------------------------------------------------------------------------
// ���̕`��
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawLine(Vector pos1, Vector pos2, XMVECTORF32  color)
{
	const VertexType V[2] =
	{
		VertexType(pos1, color,SimpleMath::Vector2(0,0)),
		VertexType(pos2, color,SimpleMath::Vector2(1,1)),
	};

	m_primitiveBatch->DrawLine(V[0], V[1]);
}

//--------------------------------------------------------------------------------
// ��`�̕`�揈��
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawBox(Vector pos1, Vector pos2, XMVECTORF32  color, SimpleMath::Vector2 uvScale)
{
	SimpleMath::Vector3 p1 = pos1;
	SimpleMath::Vector3 p2 = pos2;

	const VertexType V[4] =
	{
		VertexType(p1,									color,	SimpleMath::Vector2(0,0)),
		VertexType(SimpleMath::Vector3(p2.x,p1.y,p2.z),	color,	SimpleMath::Vector2(uvScale.x,0)),
		VertexType(p2,									color,	SimpleMath::Vector2(uvScale.x,uvScale.y)),
		VertexType(SimpleMath::Vector3(p1.x,p2.y,p1.z),	color,	SimpleMath::Vector2(0,uvScale.y)),
	};

	m_primitiveBatch->DrawQuad(V[0], V[1], V[2], V[3]);
}

//--------------------------------------------------------------------------------
// �~�̕`�揈��
//--------------------------------------------------------------------------------
void ShapeRender2D::DrawCircle(Vector centerPos, float radius, XMVECTORF32  color)
{
	SimpleMath::Vector2 pos;
	XMStoreFloat2(&pos, centerPos);					//	XMVECTOR��Vector2�ɕϊ�

	const int DIV					= 64;			//	������
	const int VERTEX_COUNT			= DIV + 1;		//	���_���i�~�� + ���S�j
	const int POLYGON_VERT_COUNT	= 3;			//	1�|���S���ɑ΂��钸�_��

	VertexType		verticies[VERTEX_COUNT];			//	���_�o�b�t�@
	uint16_t		indicies[DIV * POLYGON_VERT_COUNT];	//	�C���f�b�N�X�o�b�t�@

	//	���S�̒��_��ݒ�		��XMStoreFloat3�֐��� ����1�� ����2���R�s�[����
	XMStoreFloat3(&verticies[0].position,	centerPos);			//	���W
	XMStoreFloat4(&verticies[0].color,		color);				//	�F

	//	�~����̒��_��ݒ�
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		float x = pos.x + cosf(XM_2PI / DIV * i) * radius;		//	X���W
		float y = pos.y + sinf(XM_2PI / DIV * i) * radius;		//	Y���W

		verticies[i].position = SimpleMath::Vector3(x, y, 0.0f);			//	���W�̐ݒ�
		XMStoreFloat4(&verticies[i].color, color);				//	�F
	}

	//	�C���f�b�N�X���w��
	for (int i = 0; i < DIV; i++)
	{
		indicies[i * POLYGON_VERT_COUNT + 0] = 0;
		indicies[i * POLYGON_VERT_COUNT + 1] = static_cast<uint16_t>(i + 1);
		indicies[i * POLYGON_VERT_COUNT + 2] = static_cast<uint16_t>(i + 2);
	}
	indicies[DIV * POLYGON_VERT_COUNT - 1] = indicies[1];		//	�Ō�̒��_�͈�����ďd�Ȃ�1�Ԃ�ݒ肷��i0�͒��S�j

	//	�`��
	m_primitiveBatch->DrawIndexed(
		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indicies,
		DIV * POLYGON_VERT_COUNT,
		verticies,
		VERTEX_COUNT
	);
}

//--------------------------------------------------------------------------------
// �e�N�X�`���̎g�p��؂�ւ�
//--------------------------------------------------------------------------------
void ShapeRender2D::SetTextureEnable(ID3D11Device* device, bool enableTexture)
{
	m_basicEffect->SetTextureEnabled(enableTexture);
	
	//	�C���v�b�g���C�A�E�g�̍Đ���
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexType>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);
}

//--------------------------------------------------------------------------------
// �e�N�X�`���̐ݒ�
//--------------------------------------------------------------------------------
void ShapeRender2D::SetTexture(ID3D11ShaderResourceView* texture)
{
	m_basicEffect->SetTexture(texture);
}
