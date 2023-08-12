#pragma once
#include <math.h>
#include <algorithm>

//	線形補間関数
inline float Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
inline int Lerp(int a, int b, float t)
{
	return (int)(a + t * (b - a));
}

//	上限と下限を制限
inline float Clump(float value, float Min, float Max)
{
	return std::min(std::max(value, Min), Max);
}
//	入力された値を0～1の間に制限
inline float Clump01(float value)
{
	return std::min(std::max(value, 0.0f), 1.0f);
}

//	入力された値の間でリピートする（float）
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
//	入力された値の間でリピートする（length - 1）
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

//	入力された浮動小数が近似しているかを判定する
inline bool Approximately(const float& a, const float& b)
{
	return (fabsf(a - b) < FLT_EPSILON);
}

//	入力された数値の桁数を取得する
inline int GetDigits(int num)
{
	//	戻り値
	int ret = 0;

	//	0になるまで10で割る
	while (num > 0)
	{
		num /= 10;

		//	桁数を加算
		ret++;
	}

	//	終了
	return ret;
}

//	出力タブに対してログを出力するマクロ
#define OUTPUT_LOG(formatStr, ...) _RPTN(_CRT_WARN, formatStr, __VA_ARGS__)