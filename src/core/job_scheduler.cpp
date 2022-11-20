
#include <pch.hpp>
#include <gyn/job_scheduler.hpp>

static constexpr bool ForceMainThread = false;
static const uint32 WorkerCount = ForceMainThread ? 0 : std::thread::hardware_concurrency() - 1;

JobScheduler::JobScheduler() :
    m_dataPool(32 * 1024),
    m_workers(WorkerCount),
    m_jobs(MaxJobCount),
    m_freeJobs(MaxJobCount),
    m_jobQueue(MaxJobCount)
{
    for (uint32 i = 0; i < MaxJobCount; ++i)
    {
        m_freeJobs.Push(i);
    }

    m_isRunning = true;

    for (usize i = 0; i < m_workers.size(); ++i)
    {
        m_workers[i] = std::thread([this, i]()
        {
            WorkerMain(ToU32(i + 1));
        });
    }
}

JobScheduler::~JobScheduler()
{
    Join();
}

void JobScheduler::Join()
{
    m_isRunning = false;
    for (auto& worker : m_workers)
    {
        worker.join();
    }
}

void JobScheduler::Submit(JobFunc func, void* data)
{
    AssertRunning();

    if constexpr (ForceMainThread)
    {
        func(data, 0, m_dataPool);
    }
    else 
    {
        uint32 jobIndex{};
        while (!m_freeJobs.TryPop(jobIndex))
        {
        }

        JobData job = {};
        job.func = func;
        job.data = data;

        m_jobs[jobIndex] = std::move(job);
        m_jobQueue.Push(jobIndex);
    }
}

bool JobScheduler::TryRunOnePendingJob(uint32 workerIndex)
{
    uint32 jobIndex{};
    if (m_jobQueue.TryPop(jobIndex))
    {
        JobData& job = m_jobs[jobIndex];
        job.func(job.data, workerIndex, m_dataPool);

        m_freeJobs.Push(jobIndex);

        return true;
    }
    else
    {
        return false;
    }
}

void JobScheduler::Wait(const std::atomic_int32_t& signal)
{
    uint32 s = signal.load();
    while (s != 0)
    {
        ASSERT(s > 0);

        if (!TryRunOnePendingJob(0))
        {
            ThreadYield();
        }

        s = signal.load();
    }

    ASSERT(s == 0);
}

uint32 JobScheduler::GetWorkerCount() const
{
    AssertRunning();
    return WorkerCount;
}

void JobScheduler::AssertRunning() const
{
    ASSERT(m_isRunning);
}

void JobScheduler::WorkerMain(uint32 workerIndex)
{
    while (m_isRunning)
    {
        while (TryRunOnePendingJob(workerIndex))
        {
        }

        uint32 sleepingWorkerCount = ToU32(++m_sleepingWorkerCount);
        uint32 awakeWorkerCount = WorkerCount - sleepingWorkerCount;

        // Ensure there is always at least one worker that does not sleep
        if (awakeWorkerCount > 0)
        {
            ThreadSleep1us();
        }
        else
        {
            ThreadYield();
        }

        --m_sleepingWorkerCount;
    }
}