#include "timer.h"

Timer::~Timer() {
    if (!m_stopTimer) {
        stop_timer();
    }
}

void Timer::stop_timer() {
    m_endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(m_endTime - m_startTime).count();
    m_stopTimer = true;
    float time_duration = duration * 0.001f;
    TimerStruct result{ m_name, time_duration };
    if (m_callback) m_callback(result);
}
