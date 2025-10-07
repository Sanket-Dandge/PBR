#pragma once

#include <chrono>
#include <functional>

struct TimerStruct {
    const char* name;
    float time_duration;
};

class Timer {
    public:
        template<typename Func>
        Timer(const char* name, Func&& callback)
            : m_name(name), m_stopTimer(false), m_callback(std::forward<Func>(callback)) {
            m_startTime = std::chrono::high_resolution_clock::now();
        }
        void stop_timer();
        virtual ~Timer();

    private:
        const char* m_name;
        bool m_stopTimer;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
        std::function<void(TimerStruct)> m_callback;

};
