#pragma once

#include <gyn/common.hpp>

namespace Gyn
{
    class TimeManager
    {
    public:

        TimeManager() = default;

        bool IsNull() const;

        void StartTiming();

        void Update();

        float GetDeltaTime();

        float GetElapsedTime();

        ea::span<const float> GetFPSSamples() const;

        uint32 GetFPS() const;

        uint32 GetMinFPS() const;

        uint32 GetMaxFPS() const;

        uint32 GetAverageFPS() const;

    private:

        static constexpr uint64 NullStart = Uint64Max;

        void AssertNotNull() const;

        uint64 m_start = NullStart;
        uint64 m_last = {};
        float m_dt = {};
        float m_elapsedTime = {};

        uint m_frameCount = {};
        float m_frameTimer = {};
        ea::vector<float> m_fpsSamples = {};
        float m_nullFPSSample = {}; // Returns a span to this value when fpsSample array is empty

        uint32 m_fps = {};
        uint32 m_minFPS = {};
        uint32 m_maxFPS = {};
        uint32 m_averageFPS = {};
    };
}