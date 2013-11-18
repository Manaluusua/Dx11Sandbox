#include "EnvironmentInfo.h"


namespace Dx11Sandbox
{

	double EnvironmentInfo::m_accumulatedTime = 0;
	float EnvironmentInfo::m_deltaTime = 0;

	uint32_t EnvironmentInfo::m_screenWidth = 0;
	uint32_t EnvironmentInfo::m_screenHeight = 0;

	float EnvironmentInfo::getDeltaTime()
	{
		return m_deltaTime;
	}

	double EnvironmentInfo::getTime()
	{
		return m_accumulatedTime;
	}

	uint32_t EnvironmentInfo::getScreenWidth()
	{
		return m_screenWidth;
	}

	uint32_t EnvironmentInfo::getScreenHeight()
	{
		return m_screenHeight;
	}
}