#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <mutex>
#include <string>
#include <unordered_map>
#include <limits>

#include "Core/Log.h"

namespace Cober {

    struct ProfileStats
    {
        uint64_t CallCount = 0;
        double TotalMs = 0.0;
        double MinMs = std::numeric_limits<double>::max();
        double MaxMs = 0.0;

        double AverageMs() const
        {
            return CallCount == 0 ? 0.0 : TotalMs / static_cast<double>(CallCount);
        }
    };

    class Profiler
    {
    public:
        static void Record(const char* name, double elapsedMs)
        {
            std::scoped_lock lock(GetMutex());
            auto& stats = GetData()[name];
            stats.CallCount++;
            stats.TotalMs += elapsedMs;
            stats.MinMs = std::min(stats.MinMs, elapsedMs);
            stats.MaxMs = std::max(stats.MaxMs, elapsedMs);
        }

        static const std::unordered_map<std::string, ProfileStats>& GetStats()
        {
            return GetData();
        }

        static void Clear()
        {
            std::scoped_lock lock(GetMutex());
            GetData().clear();
        }

        inline void DumpProfileStatsToLog(double minAvgMs = 0.05)
        {
            for (const auto& [name, stats] : Cober::Profiler::GetStats())
            {
                if (stats.AverageMs() < minAvgMs)
                    continue;

                LOG_CORE_INFO("{} | calls={} total={:.3f}ms avg={:.3f}ms min={:.3f}ms max={:.3f}ms",
                    name,
                    stats.CallCount,
                    stats.TotalMs,
                    stats.AverageMs(),
                    stats.MinMs,
                    stats.MaxMs);
            }
        }

    private:
        static std::unordered_map<std::string, ProfileStats>& GetData()
        {
            static std::unordered_map<std::string, ProfileStats> s_Data;
            return s_Data;
        }

        static std::mutex& GetMutex()
        {
            static std::mutex s_Mutex;
            return s_Mutex;
        }
    };

    
    class ScopeTimer
    {
    public:
        explicit ScopeTimer(const char* name)
            : m_Name(name), m_Start(std::chrono::high_resolution_clock::now())
        {
        }

        ~ScopeTimer()
        {
            auto end = std::chrono::high_resolution_clock::now();
            double elapsedMs = std::chrono::duration<double, std::milli>(end - m_Start).count();
            Profiler::Record(m_Name, elapsedMs);
        }

    private:
        const char* m_Name;
        std::chrono::high_resolution_clock::time_point m_Start;
    };
}

#endif