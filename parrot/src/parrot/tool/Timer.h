#pragma once

#include <cstdint>

namespace parrot {

    static constexpr inline int32_t TIMESTEP    = 16;
    static constexpr inline int32_t TIMESTEP_SI = TIMESTEP * 1000;

    constexpr int32_t seconds(const int32_t time) { return time * 1000;                          }
    constexpr int32_t seconds(const float   time) { return static_cast<int32_t>(time) * 1000;    }
    constexpr int32_t minutes(const int32_t time) { return time * 60000;                         }
    constexpr int32_t minutes(const float   time) { return static_cast<int32_t>(time) * 60000;   }
    constexpr int32_t hours  (const int32_t time) { return time * 3600000;                       }
    constexpr int32_t hours  (const float   time) { return static_cast<int32_t>(time) * 3600000; }

    constexpr int32_t operator ""_s(unsigned long long   time) { return static_cast<int32_t>(time * 1000   ); }
    constexpr int32_t operator ""_s(long double          time) { return static_cast<int32_t>(time * 1000   ); }
    constexpr int32_t operator ""_m(unsigned long long   time) { return static_cast<int32_t>(time * 60000  ); }
    constexpr int32_t operator ""_m(long double          time) { return static_cast<int32_t>(time * 60000  ); }
    constexpr int32_t operator ""_h(unsigned long long   time) { return static_cast<int32_t>(time * 3600000); }
    constexpr int32_t operator ""_h(long double          time) { return static_cast<int32_t>(time * 3600000); }

    class Timer {
    public:
        Timer() {};
        Timer(const int32_t time) : m_remaining(time), m_time(time) {};

        inline void reset(const int32_t time = 0) {
            m_time      = time;
            m_remaining = m_time;
        }

        inline void restart() {
            reset(m_time);
        }

        inline bool update(const int32_t frame_time, bool should_restart = true) {
            if (expired()) {
                return false;
            }
            m_remaining -= frame_time;
            bool just_expired = expired();
            if (just_expired && should_restart) {
                restart();
            }
            return just_expired;
        }

        inline int32_t remaining() const {
            return m_remaining;
        }

        inline int32_t total() const {
            return m_time;
        }

        inline void stop() {
            m_time = 0;
        }

        inline bool expired() const {
            return m_remaining <= 0;
        }

        inline float percentage() const {
            return m_time <= 0 ? 0 : static_cast<float>(m_remaining) / static_cast<float>(m_time);
        }

        inline Timer& operator+=(const int32_t time) {
            m_remaining += time;
            return *this;
        }

        inline Timer& operator-=(const int32_t time) {
            m_remaining -= time;
            return *this;
        }

    private:
        int32_t m_time = 0;
        int32_t m_remaining = 0;
    };

}
