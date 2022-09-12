#pragma once
#include <zephyr.h>

class Timer {
private:
    static const uint8_t MAX_TIMER_NUM = 10;
    static Timer* timerPool[MAX_TIMER_NUM];
    static void TimerCallback(k_timer* timer_id);
    static void WorkCallback(k_work* work_id);
    bool initialized = false;
    /** timer object */
    k_timer timer;
    /** work object */
    k_work work;
    /** callback func */
    void (*callback)(void *);
public:
    Timer(void (*callback)(void *));
    ~Timer();
    void Init();
    void Start(uint32_t ms);
    void Start(uint32_t duration_ms, uint32_t period_ms);
    void Stop();
};