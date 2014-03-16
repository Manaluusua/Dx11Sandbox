#include "SceneManager.h"
#include "EnvironmentInfo.h"
namespace Dx11Sandbox
{
	SceneManager::SceneManager(void)
	{

	}

	SceneManager::~SceneManager(void)
	{

	}

	void SceneManager::setEnvironmentTime(float time)
	{
		EnvironmentInfo::m_accumulatedTime = time;
		
	}
	void SceneManager::setEnvironmentDeltaTime(float dt)
	{
		EnvironmentInfo::m_deltaTime = dt;
	}
	void SceneManager::setEnvironmentScreenDimension(float w, float h)
	{
		EnvironmentInfo::m_screenWidth = w;
		EnvironmentInfo::m_screenHeight = h;
	}
}