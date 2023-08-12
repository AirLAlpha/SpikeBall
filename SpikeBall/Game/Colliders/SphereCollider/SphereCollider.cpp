/**********************************************
 *
 *  SphereCollider.cpp
 *  ����̃R���C�_�[
 *
 *  ����ҁF���ە��m
 *  ������F2023/03/28
 *
 **********************************************/
#include "pch.h"
#include "SphereCollider.h"
#include "DebugDraw/DebugDraw.h"
#include "Colliders/PlaneCollider/PlaneCollider.h"
#include "Colliders/Line.h"

using namespace DirectX;


//	�R���X�g���N�^
SphereCollider::SphereCollider(
	const SimpleMath::Vector3& pos,
	const float& radius
):
	m_pos(pos),
	m_radius(radius)
{
}

//	�f�X�g���N�^
SphereCollider::~SphereCollider()
{
}

//--------------------------------------------------------------------------------
// �������ʂƂ̏Փ˔���
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const SimpleMath::Plane& plane) const
{
	//	���ʂ܂ł̋������擾
	float dist = plane.DotCoordinate(m_pos);
	//	���a�����ʂ܂ł̋����ȉ��Ȃ�Փ˂��Ă���
	if (fabsf(dist) < m_radius)
		return true;

	return false;
}

//--------------------------------------------------------------------------------
// �����Ƃ̏Փ˔���
// �Q�l	�Fhttp://marupeke296.com/COL_3D_No1_PointToLine.html
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const Line& line) const
{
	//	�����̃x�N�g�����擾
	SimpleMath::Vector3 lineVec = line.GetNormalizedVector();
	//	�����̒������擾
	float lineLength = line.GetLength();
	//	�����̎n�_�Ǝ��g�̍��W�Ƃ̃x�N�g��
	SimpleMath::Vector3 lineToPos = m_pos - line.GetPosA();

	//	�����̃x�N�g���i���K���ς݁j�Ɛ����̒��_���玩�M�܂ł̃x�N�g���̓��ς��v�Z
	//	�v�Z�������ς𒷂��Ŋ����āA������̊������v�Z
	float t = lineVec.Dot(lineToPos) / lineLength;
	if (t > 1.0f || t < 0.0f)
	{
		//	�����̏�ɂȂ��Ƃ��͐����̒��_�Ƃ̋������画�肷��
		SimpleMath::Vector3 lineVert;
		//	�߂����_���擾
		if (t > 1.0)
			lineVert = line.GetPosB();
		else
			lineVert = line.GetPosA();

		//	���W�ƒ��_�̋�����2����v�Z
		float dist = SimpleMath::Vector3::DistanceSquared(lineVert, m_pos);
		//	���������a��2���菬�����Ƃ��͏Փ˂��Ă���Ƃ���
		if (dist <= m_radius * m_radius)
			return true;
		else
			return false;
	}

	//	����������W���v�Z���A���g�Ƃ̋������擾
	float h = (line.GetVector() * t - lineToPos).LengthSquared();
	//	���������a���傫���Ƃ��͏Փ˂��Ă��Ȃ��Ƃ��ďI��
	if (h > m_radius * m_radius)
		return false;

	//	���ׂĂ̏������p�X�����I�u�W�F�N�g�͏Փ˂����Ƃ��ďI��
	return true;
}

//--------------------------------------------------------------------------------
// �X�t�B�A�R���C�_�[���m�̏Փ˔���
//--------------------------------------------------------------------------------
bool SphereCollider::CheckHit(const SphereCollider& other) const
{
	//	�߂�l
	bool ret = false;

	//	�ΏۂƂȂ�I�u�W�F�N�g�ւ̃x�N�g�����쐬
	SimpleMath::Vector3 toOtherVec = other.m_pos - m_pos;
	//	�x�N�g���̒����̓����v�Z
	float distSquare = toOtherVec.x * toOtherVec.x + toOtherVec.y * toOtherVec.y + toOtherVec.z * toOtherVec.z;
	//	�Փ˂����Ƃ��鋗���̌v�Z
	float hitDistSquare = (m_radius + other.m_radius) * (m_radius + other.m_radius);

	//	�x�N�g���̒������Փ˂����Ƃ��鋗�����Z���Ƃ��͏Փ˂��Ă���Ƃ���
	if (distSquare <= hitDistSquare)
		ret = true;

	return ret;
}


#if _DEBUG
//--------------------------------------------------------------------------------
// �f�o�b�O�p�̏���������
//--------------------------------------------------------------------------------
void SphereCollider::DebugInit(ID3D11Device* device, ID3D11DeviceContext* context)
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
// �f�o�b�O�p�̕`�揈��
//--------------------------------------------------------------------------------
void SphereCollider::DebugRender(
	ID3D11DeviceContext* context, 
	const CommonStates& states, 
	const SimpleMath::Matrix& view, 
	const SimpleMath::Matrix& proj
)
{
	//	�C���v�b�g���C�A�E�g�̐ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	//	�s��̐ݒ�
	m_basicEffect->SetWorld(SimpleMath::Matrix::Identity);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	//	�V�F�[�_�[�̓K��
	m_basicEffect->Apply(context);

	//	�[�x�o�b�t�@���g�p����悤�ɐݒ�
	context->OMSetDepthStencilState(states.DepthDefault(), 0);

	//	�g�̕`��
	m_primitiveBatch->Begin();
	DX::Draw(
		m_primitiveBatch.get(),
		DirectX::BoundingSphere(m_pos, m_radius),
		Colors::LightGreen
	);
	m_primitiveBatch->End();

}
#endif