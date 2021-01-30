#pragma once

#include <chrono>

namespace parrot {

    template<typename Func>
    class ScopeTimer {
    public:
        ScopeTimer(const char* name, Func&& function ) : m_name(name), m_function(function), m_stopped(false) {
            m_start_time_point = std::chrono::high_resolution_clock::now();
        }

        ~ScopeTimer() {
            if (!m_stopped) {
                stop();
            }
        }

        void stop() {
            auto end_time_point = std::chrono::high_resolution_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time_point).time_since_epoch().count();
            long long end   = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point    ).time_since_epoch().count();

            m_stopped = true;
            float duration = (end - start) * 0.001f;
            m_function({ m_name, duration });
        }

    private:
        const char* m_name;
        Func m_function;
        bool m_stopped;
        std::chrono::time_point<std::chrono::steady_clock> m_start_time_point;
    };

}