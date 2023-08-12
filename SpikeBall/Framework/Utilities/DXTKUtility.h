#pragma once

#include <SimpleMath.h>
#include <random>
#include "UtilityFunction.h"

//--------------------------------------------------------------------------------
// �傫��1�̃����_���ȃx�N�g���̐���
//--------------------------------------------------------------------------------
inline DirectX::SimpleMath::Vector3 GetRandomVec()
{
	using namespace DirectX::SimpleMath;

	float x, y, z;
	x = ((float)rand() / RAND_MAX * 2) - 1.0f;
	y = ((float)rand() / RAND_MAX * 2) - 1.0f;
	z = ((float)rand() / RAND_MAX * 2) - 1.0f;

	Vector3 ret = Vector3(x, y, z);
	ret.Normalize();

	return ret;
}

//--------------------------------------------------------------------------------
// �傫��1�̃����_���Ȑ�Βl�x�N�g���̐���
//--------------------------------------------------------------------------------
inline DirectX::SimpleMath::Vector3 GetRandomAbsVec()
{
	using namespace DirectX::SimpleMath;

	Vector3 ret = GetRandomVec();
	ret.x = fabsf(ret.x);
	ret.y = fabsf(ret.y);
	ret.z = fabsf(ret.z);

	return ret;
}

//--------------------------------------------------------------------------------
// �萔�o�b�t�@�̑傫�����擾
//--------------------------------------------------------------------------------
template<typename T>
inline UINT GetConstantBufferSize()
{
	UINT size = sizeof(T);
	size += size % 16;

	return size;
}

//--------------------------------------------------------------------------------
// Vector3���ߎ����Ă��邩�𔻒肷��
//--------------------------------------------------------------------------------
inline bool Approximately(const DirectX::SimpleMath::Vector3& a, const DirectX::SimpleMath::Vector3& b)
{
	bool x = Approximately(a.x, b.x);
	bool y = Approximately(a.y, b.y);
	bool z = Approximately(a.z, b.z);

	return (x && y && z);
}
