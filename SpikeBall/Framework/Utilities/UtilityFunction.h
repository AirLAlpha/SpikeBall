#pragma once
#include <math.h>
#include <algorithm>

//	���`��Ԋ֐�
inline float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
inline int Lerp(int a, int b, float t)
{
	return (int)(a + t * (b - a));
}

//	����Ɖ����𐧌�
inline float Clump(float value, float Min, float Max)
{
	return std::min(std::max(value, Min), Max);
}
//	���͂��ꂽ�l��0�`1�̊Ԃɐ���
inline float Clump01(float value)
{
	return std::min(std::max(value, 0.0f), 1.0f);
}

//	���͂��ꂽ�l�̊ԂŃ��s�[�g����ifloat�j
inline float Repeat(float t, float length)
{
	if (t < 0)
	{
		float diff = fmodf(fabsf(t), length);
		return length - diff;
	}
	else
	{
		return fmodf(t, length);
	}
}
//	���͂��ꂽ�l�̊ԂŃ��s�[�g����ilength - 1�j
inline int Repeat(int t, int length)
{
	if (t < 0)
	{
		int diff = abs(t) % length;
		return length - diff;
	}
	else
	{
		return t % length;
	}
}

//	���͂��ꂽ�����������ߎ����Ă��邩�𔻒肷��
inline bool Approximately(const float& a, const float& b)
{
	return (fabsf(a - b) < FLT_EPSILON);
}

//	���͂��ꂽ���l�̌������擾����
inline int GetDigits(int num)
{
	//	�߂�l
	int ret = 0;

	//	0�ɂȂ�܂�10�Ŋ���
	while (num > 0)
	{
		num /= 10;

		//	���������Z
		ret++;
	}

	//	�I��
	return ret;
}

//	�o�̓^�u�ɑ΂��ă��O���o�͂���}�N��
#define OUTPUT_LOG(formatStr, ...) _RPTN(_CRT_WARN, formatStr, __VA_ARGS__)