#ifndef DX11SANDBOX_SCENEINFO_H
#define DX11SANDBOX_SCENEINFO_H

#include <cstdint>

namespace Dx11Sandbox
{
	class EnvironmentInfo
	{
		friend class SceneManager;
	public:

		static double getTime();
		static float getDeltaTime();

		static uint32_t getScreenWidth();
		static uint32_t getScreenHeight();

	protected:
		static double m_accumulatedTime;
		static float m_deltaTime;

		static uint32_t m_screenWidth;
		static uint32_t m_screenHeight;
	};


	
};

#endif