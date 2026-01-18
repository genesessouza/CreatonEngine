#include "Engine/Core/Log/Logger.h"

#include "Engine/Core/Time/Time.h"

namespace Engine::Core::Time
{
    static Time::TimeProvider s_TimeProvider = nullptr;
    static double s_LastTime = 0.0;

    void Time::SetProvider(TimeProvider provider)
    {
        CRTN_ASSERT(provider, "Time provider cannot be null!");
        s_TimeProvider = provider;
        s_LastTime = provider();
    }

    double Time::GetTime()
    {
        CRTN_ASSERT(s_TimeProvider, "Time provider not set!");
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
