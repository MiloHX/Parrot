#pragma once

namespace parrot {
    
    class TimeStep {
    public:
        TimeStep(float time = 0.0f) : m_time(time) {}

        float getSeconds     () const { return m_time;           }
        float getMilliseconds() const { return m_time * 1000.0f; }

        operator float() const { return m_time; }

    private:
        float m_time;
    };
}