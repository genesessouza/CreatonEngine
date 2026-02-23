#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Time/Time.h"

namespace Engine::Core::Time
{
	static Time::TimeProvider s_TimeProvider = nullptr;
	static double s_LastTime = 0.0;

	void Time::SetProvider(TimeProvider provider)
	{
		if (!provider)
		{
			CRTN_LOG_CRITICAL("<Time::SetProvider>: [s_TimeProvider] is null!");
			CRTN_ASSERT(!provider, "<Time::SetProvider>: Time provider not initialized!");
			return;
		}

		s_TimeProvider = provider;
		s_LastTime = provider();
	}

	double Time::GetTime()
	{
		if (!s_TimeProvider)
		{
			CRTN_LOG_CRITICAL("<Time::GetTime>: [s_TimeProvider] is null!");
			CRTN_ASSERT(!s_TimeProvider, "<Time::GetTime>: Time provider not set!");
			
			return 0;
		}

		return s_TimeProvider();
	}

	float Time::GetDeltaTime()
	{
		double now = GetTime();
		float delta = static_cast<float>(now - s_LastTime);
		s_LastTime = now;
		return delta;
	}
}
