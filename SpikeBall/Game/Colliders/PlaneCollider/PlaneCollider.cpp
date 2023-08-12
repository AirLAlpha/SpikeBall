/**********************************************
 *
 *  PlaneCollider.cpp
 *  ��̃R���C�_�[
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/29
 *
 **********************************************/
#include "pch.h"
#include "PlaneCollider.h"
#include "DebugDraw/DebugDraw.h"
#include "Colliders/SphereCollider/SphereCollider.h"
#include "Colliders/Line.h"
#include "Utilities/DXTKUtility.h"

using namespace DirectX;

#if _DEBUG
//	���_���W
const SimpleMath::Vector3 PlaneCollider::VERTICIES[PlaneCollider::VERTEX_COUNT] =
{
	SimpleMath::Vector3(-0.5f, 0.0f, 0.5f),
	SimpleMath::Vector3(0.5f, 0.0f, 0.5f),
	SimpleMath::Vector3(0.5f, 0.0f, -0.5f),
	SimpleMath::Vector3(-0.5f, 0.0f, -0.5f)
};
#endif

//	�R���X�g���N�^
PlaneCollider::PlaneCollider(
	const SimpleMath::Vector3& pos, 
	const SimpleMath::Vector2& size, 
	const SimpleMath::Quaternion& rot, 
	const SimpleMath::Vector3& normal
):
	m_pos(pos),
	m_size(size),
	m_rot(rot),
	m_normal(normal)
{
	//	���ʂ̏�����
	m_plane = SimpleMath::Plane(pos, normal);
}

//	�f�X�g���N�^
PlaneCollider::~PlaneCollider()
{
}


//--------------------------------------------------------------------------------
// �X�t�B�A�R���C�_�[�Ƃ̏Փ˔���
//--------------------------------------------------------------------------------
bool PlaneCollider::CheckHit(const SphereCollider& other, SimpleMath::Vector3* hitPoint) const
{
	//	�������ʂɏՓ˂��Ă��邩���m�F����
	bool hitPlane = other.CheckHit(m_plane);
	//	�������ʂɏՓ˂��Ă��Ȃ��Ƃ��͏Փ˂��Ă��Ȃ��Ƃ��ďI��
	if (!hitPlane)
		return false;

	//	���W���S���������͏Փ˂��Ă���Ƃ���
	if (Approximately(m_pos, other.GetPosition()))
		return true;

	//	���ʂ̉�]��g�k�̕ϊ��s����쐬
	SimpleMath::Matrix vMat =
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	other��ʂ镽�ʂɐ����Ȑ����̍쐬
	SimpleMath::Vector3 p1 = other.GetPosition() + m_normal * other.GetRadius();		//	�����̒��_1
	SimpleMath::Vector3 p2 = other.GetPosition() + -m_normal * other.GetRadius();		//	�����̒��_2
	Line l = Line(p1, p2);

	//	�����Ƃ̏Փ˔�����Ƃ�Փ˂��Ă�����I��
	if (CheckHit(l, hitPoint))
		return true;

	//	���_
	const SimpleMath::Vector3 v[] =
	{
		SimpleMath::Vector3(-0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,0.5f),
		SimpleMath::Vector3(-0.5f,0.0f,0.5f),
	};
	//	�ӂƋ��̓����蔻��
	for (int i = 0; i < 4; i++)
	{
		int index2 = Repeat(i + 1, 4);
		SimpleMath::Vector3 v1 = SimpleMath::Vector3::Transform(v[i], vMat);
		SimpleMath::Vector3 v2 = SimpleMath::Vector3::Transform(v[index2], vMat);

		//	�����̃I�u�W�F�N�g�𐶐�
		Line l2 = Line(v1, v2);
		//	���Ƃ̔�����擾
		bool edgeHit = other.CheckHit(l2);

		//	�����ꂩ�̐����ƏՓ˂��Ă����炠�����Ă���Ƃ��ďI��
		if (edgeHit)
			return true;
	}

	//	���ׂĂ̏����ɏՓ˂��Ȃ��Ƃ��́A�|���S���̓����ɒ��_�����݂��Ȃ��Ƃ��ďI��
	return false;
}

//--------------------------------------------------------------------------------
// �����Ƃ̏Փ˔���
//--------------------------------------------------------------------------------
bool PlaneCollider::CheckHit(const Line& line, SimpleMath::Vector3* hitPoint) const
{
	//	���ʂ̉�]��g�k�̕ϊ��s����쐬
	SimpleMath::Matrix vMat = 
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) *
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) *
		SimpleMath::Matrix::CreateTranslation(m_pos);

	SimpleMath::Vector3 p1 = line.GetPosA();
	SimpleMath::Vector3 p2 = line.GetPosB();

	//	���ʂ�1���_�̍��W���v�Z
	SimpleMath::Vector3 p0 = SimpleMath::Vector3::Transform(SimpleMath::Vector3(-0.5f, 0.0f, -0.5f), vMat);		//	���ʂ�1���_�̍��W

	//	���ʂ̒��_��������܂ł̃x�N�g�����쐬
	SimpleMath::Vector3 v1 = p1 - p0;
	SimpleMath::Vector3 v2 = p2 - p0;

	//	�����̒��_���畽�ʂ܂ł̋������擾
	float p1_dist = fabsf(m_plane.DotCoordinate(p1));
	float p2_dist = fabsf(m_plane.DotCoordinate(p2));

	//	�����_�i��_�j�܂ł̊������v�Z
	float a = p1_dist / (p1_dist + p2_dist);
	float b = p2_dist / (p1_dist + p2_dist);

	//	��_�̍��W���v�Z
	SimpleMath::Vector3 v3 = (b * v1 + a * v2) + p0;

	if (hitPoint != nullptr)
		*hitPoint = v3;

	//	���_
	const SimpleMath::Vector3 v[] =
	{
		SimpleMath::Vector3(-0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,-0.5f),
		SimpleMath::Vector3(0.5f,0.0f,0.5f),
		SimpleMath::Vector3(-0.5f,0.0f,0.5f),
	};
	//	�C���f�b�N�X
	const int indcies[] =
	{
		0, 1, 2,
		0, 2, 3,
	};
	//	1�|���S���ɑ΂��钸�_�̐�
	const int POLYGON_VERT_COUNT = 3;
	//	�|���S���̐�
	const int POLYGON_COUNT = 2;
	//	�O�ς̌��ʂ�ێ�����ϐ�
	SimpleMath::Vector3 crosses[POLYGON_VERT_COUNT];

	//	��_�̍��W�����ʂ̓����ɂ��邩���m�F����
	for (int i = 0; i < POLYGON_COUNT; i++)
	{
		//	�e���_���m�̃x�N�g���ƌ�_�܂ł̃x�N�g���̊O�ς��v�Z����
		for (int j = 0; j < POLYGON_VERT_COUNT; j++)
		{
			int index = j + i * POLYGON_VERT_COUNT;
			int index2 = Repeat(index + 1, POLYGON_VERT_COUNT) + i * POLYGON_VERT_COUNT;

			//	���ꂼ��̒��_�𕽖ʂ̉�]��傫���ɍ��킹�Ĉړ�������
			SimpleMath::Vector3 v4 = SimpleMath::Vector3::Transform(v[indcies[index]], vMat);
			SimpleMath::Vector3 v5 = SimpleMath::Vector3::Transform(v[indcies[index2]], vMat);
			//	�ӂ̃x�N�g�����쐬
			SimpleMath::Vector3 vertVec = v5 - v4;
			//	�ӂ̒��_�ƌ�_�܂ł̃x�N�g�����쐬
			SimpleMath::Vector3 pointVec = v3 - v5;

			//	�O�ς̌v�Z���s��
			SimpleMath::Vector3 crossResult;
			pointVec.Cross(vertVec, crossResult);
			crossResult.Normalize();
			//	���ʂ�ێ�
			crosses[j] = crossResult;
		}

		//	�|���S���̓����ɒ��_�����݂���ΏՓ˂��Ă���Ƃ��ďI��
		float d0 = SimpleMath::Vector3::DistanceSquared(crosses[0], m_normal);
		float d1 = SimpleMath::Vector3::DistanceSquared(crosses[1], m_normal);
		float d2 = SimpleMath::Vector3::DistanceSquared(crosses[2], m_normal);
		if (
			d0 <= FLT_EPSILON &&
			d1 <= FLT_EPSILON &&
			d2 <= FLT_EPSILON
			)
		{
			return true;
		}
	}

	//	�|���S���̓����Ɍ�_�����݂��Ȃ��Ƃ��͏Փ˂��Ă��Ȃ�
	return false;
}


#if _DEBUG
//--------------------------------------------------------------------------------
// �f�o�b�O�p����������
//--------------------------------------------------------------------------------
void PlaneCollider::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
{
	//	�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	//	�C���v�b�g���C�A�E�g�̍쐬
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(device, m_basicEffect.get(), m_inputLayout.ReleaseAndGetAddressOf())
	);

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
}

//--------------------------------------------------------------------------------
// �f�o�b�O�p�`�揈��
//--------------------------------------------------------------------------------
void PlaneCollider::DebugRender(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	�C���v�b�g���C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	//	���[���h�s��̍쐬
	SimpleMath::Matrix world = 
		SimpleMath::Matrix::CreateScale(m_size.x, 1.0f, m_size.y) * 
		SimpleMath::Matrix::CreateFromQuaternion(m_rot) * 
		SimpleMath::Matrix::CreateTranslation(m_pos);

	//	�s��̐ݒ�
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	//	�V�F�[�_�[�̓K��
	m_basicEffect->Apply(context);

	//	�[�x�o�b�t�@���g�p����悤�ɐݒ�
	context->OMSetDepthStencilState(states.DepthDefault(), 0);

	//	�g�̕`��
	m_primitiveBatch->Begin();
	DX::DrawQuad(
		m_primitiveBatch.get(),
		VERTICIES[0],
		VERTICIES[1],
		VERTICIES[2],
		VERTICIES[3],
		Colors::LightGreen
	);
	m_primitiveBatch->End();

	//	���[���h�s������Z�b�g
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->Apply(context);

	//	�@���̕`��
	m_primitiveBatch->Begin();
	DX::DrawRay(
		m_primitiveBatch.get(),
		m_pos,
		m_normal,
		false,
		Colors::LightGreen
	);
	m_primitiveBatch->End();
}
#endif