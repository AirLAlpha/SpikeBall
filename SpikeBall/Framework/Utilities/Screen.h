#pragma once
#include <SimpleMath.h>

#define BACKGROUND_COLOR	(DirectX::Colors::DarkGray)		//	背景色

class Screen
{
public:
	const static int WIDTH = 1600;		//	画面の横幅
	const static int HEIGHT = 900;		//	画面の縦幅

	//	画面サイズをVector2で返す
	static DirectX::SimpleMath::Vector2 SCREEN_SIZE()
	{
		return { WIDTH, HEIGHT };
	}
	//	画面の中心座標を返す
	static DirectX::SimpleMath::Vector2 SCREEN_CENTER()
	{
		return { (float)WIDTH_HALF(), (float)HEIGHT_HALF() };
	}

	//	横幅の半分を返す
	static int WIDTH_HALF()
	{
		return (int)(WIDTH / 2);
	}

	//	縦幅の半分を返す
	static int HEIGHT_HALF()
	{
		return (int)(HEIGHT / 2);
	}

	//	画面の半分のサイズを返す（SCREEN_CENTERど同じ）
	static DirectX::SimpleMath::Vector2 SCREEN_HALF()
	{
		return SCREEN_CENTER();
	}
};