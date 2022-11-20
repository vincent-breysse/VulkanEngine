
#include <pch.hpp>
#include <gyn/time_manager.hpp>

static uint64 GetNow(void)
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

bool TimeManager::IsNull() const
{
    return m_start == NullStart;
}

void TimeManager::StartTiming()
{
    const auto now = GetNow();

    m_start = now;
    m_last = now;
}

void TimeManager::Update()
{
    AssertNotNull();

    const auto now = GetNow();

    ASSERT(now >= m_last);
    ASSERT(now >= m_start);

    m_dt = (now - m_last) / 1'000'000.f;
    m_elapsedTime = (now - m_start) / 1'000'000.f;
    m_last = now;

    m_frameTimer += m_dt;
    ++m_frameCount;

    if (m_frameTimer >= 1.0f)
    {
        constexpr usize MaxSampleCount = 16;

        m_fpsSamples.push_back(float(m_frameCount));
        if (m_fpsSamples.size() > MaxSampleCount)
        {
            m_fpsSamples.erase(m_fpsSamples.begin());
        }

        float averageFPS = 0.f;
        float minFPS = FloatMax;
        float maxFPS = FloatMin;
        for (float fps : m_fpsSamples)
        {
            minFPS = std::min(minFPS, fps);
            maxFPS = std::max(maxFPS, fps);
            averageFPS += fps;
        }

        m_fps = m_frameCount;
        m_maxFPS = uint32(maxFPS);
        m_minFPS = uint32(minFPS);
        m_averageFPS = uint32(averageFPS / m_fpsSamples.size());

        m_frameCount = 0;
        m_frameTimer = 0.f;
    }
}

float TimeManager::GetDeltaTime()
{
    AssertNotNull();
    return m_dt;
}

float TimeManager::GetElapsedTime()
{
    AssertNotNull();
    return m_elapsedTime;
}

ea::span<const float> TimeManager::GetFPSSamples() const
{
    AssertNotNull();

    if (m_fpsSamples.empty())
    {
        return ea::span(&m_nullFPSSample, 1);
    }
    else
    {
        return m_fpsSamples;
    }
}

uint32 TimeManager::GetFPS() const
{
    AssertNotNull();
    return m_fps;
}

uint32 TimeManager::GetMinFPS() const
{
    AssertNotNull();
    return m_minFPS;
}

uint32 TimeManager::GetMaxFPS() const
{
    AssertNotNull();
    return m_maxFPS;
}

uint32 TimeManager::GetAverageFPS() const
{
    AssertNotNull();
    return m_averageFPS;
}

void TimeManager::AssertNotNull() const
{
    ASSERT(!IsNull());
}