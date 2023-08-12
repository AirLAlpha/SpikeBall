/**********************************************
 *
 *  UserResources.h
 *  各リソースのラップクラス（シングルトン）
 *
 *  製作者：牛丸文仁
 *  制作日：2023/05/22
 *
 **********************************************/
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "CommonStates.h"

class UserResources
{
private:
	DX::DeviceResources*	m_deviceResources;		//	DeviceResources
	
	DX::StepTimer*			m_stepTimer;			//	StepTimer

	DirectX::CommonStates*	m_commonStates;			//	CommonStates

private:
	//	コンストラクタ
	UserResources():
		m_deviceResources(nullptr),
		m_stepTimer(nullptr),
		m_commonStates(nullptr)
	{};
	//	デストラクタ
	~UserResources() = default;

public:
	UserResources(const UserResources&) = delete;
	UserResources& operator=(const UserResources&) = delete;
	UserResources(const UserResources&&) = delete;
	UserResources& operator=(const UserResources&&) = delete;

	//	シングルトンインスタンスの取得
	static UserResources* GetInstance()
	{
		static UserResources instance;
		return &instance;
	}

public:
	//	DeviceResources
	void SetDeviceResources(DX::DeviceResources* deviceResources)	{ m_deviceResources = deviceResources; }
	DX::DeviceResources* GetDeviceResources()						{ return m_deviceResources; }

	//	StepTimer
	void SetStepTimer(DX::StepTimer* stepTimer)						{ m_stepTimer = stepTimer; }
	DX::StepTimer* GetStepTimer()									{ return m_stepTimer; }

	//	CommonStates
	void SetCommonStates(DirectX::CommonStates* commonStates)		{ m_commonStates = commonStates; }
	DirectX::CommonStates* GetCommonStates()						{ return m_commonStates; }
};
