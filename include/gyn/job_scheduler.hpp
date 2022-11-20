#pragma once

#include <gyn/common.hpp>
#include <gyn/common/threaded_ring_buffer.hpp>
#include <gyn/common/threaded_transient_pool.hpp>

namespace Gyn
{
    using JobFunc = void(*)(void* data, uint32 workerIndex, ThreadedTransientPool& pool);

    struct Job
    {
        void operator()(uint32) {}
    };

    // Super simple fiber-based job scheduler. 
    // TODO: Add support for job dependencies via atomic counters.
    class JobScheduler : NoCopyMove
    {
    public:

        struct JobData
        {
            JobFunc func;
            void* data;
        };

        static constexpr uint32 MaxJobCount = 512;

        JobScheduler();

        ~JobScheduler();

        void Join();

        void Submit(JobFunc func, void* data);

        bool TryRunOnePendingJob(uint32 workerIndex);

        void Wait(const std::atomic_int32_t& signal);
 
        uint32 GetWorkerCount() const;

        template <typename T>
        T& CreateJob()
        {
            static_assert(std::is_base_of_v<Job, T>, 
                "T in CreateJob<T> must inherit from Job");

            AssertRunning();
            return m_dataPool.Create<T>();
        }

        template <typename JobT>
        void Submit(JobT& job)
        {
            static_assert(std::is_base_of_v<Job, JobT>,
                "T in CreateJob<T> must inherit from Job");

            AssertRunning();

            const auto jobFunc = GetJobFunc<JobT>();
            Submit(jobFunc, &job);
        }

        template <typename JobT>
        void Run(JobT& job)
        {
            static_assert(std::is_base_of_v<Job, JobT>,
                "T in CreateJob<T> must inherit from Job");

            AssertRunning();

            const auto jobFunc = GetJobFunc<JobT>();
            jobFunc(&job, 0u, m_dataPool);
        }

    private:

        void AssertRunning() const;

        void WorkerMain(uint32 workerIndex);

        template <typename JobT>
        static auto GetJobFunc()
        {
            return [](void* data, uint32 workerIndex, ThreadedTransientPool& pool)
            {
                JobT& job = *reinterpret_cast<JobT*>(data);

                job(workerIndex);
                pool.Destroy(&job);
            };
        }

        ThreadedTransientPool m_dataPool = {};
        ea::vector<std::thread> m_workers = {};
        ea::vector<JobData> m_jobs = {};
        ThreadedRingBuffer m_freeJobs = {};
        ThreadedRingBuffer m_jobQueue = {};
        std::atomic_bool m_isRunning = {};
        std::atomic_int32_t m_sleepingWorkerCount = {};
    };
}