#include "Timer.hpp"

#include <logging/log.h>
#include "../Keyboard/Power.hpp"

LOG_MODULE_REGISTER(Timer);

Timer* Timer::timerPool[MAX_TIMER_NUM] = {nullptr};

void Timer::WorkCallback(k_work* work_id)
{
    for(int i = 0; i < MAX_TIMER_NUM; i++) {
        if(&(timerPool[i]->work) == work_id) {
            timerPool[i]->callback(nullptr);
        }
    }
}

void Timer::TimerCallback(k_timer* timer_id)
{
    k_work_submit((k_work *)(timer_id->user_data));
}

Timer::Timer(void (*callback)(void *)) {
    this->callback = callback;

    for(int i = 0; i < MAX_TIMER_NUM; i++) {
        if(timerPool[i] == nullptr) {
            timerPool[i] = this;
            return;
        }
    }

    LOG_ERR("too much timer defined");
    Power::Stop();
}

Timer::~Timer() {
    for(int i = 0; i < MAX_TIMER_NUM; i++) {
        if(timerPool[i] == this) timerPool[i] = nullptr;
    }
}

void Timer::Init() {
    if(initialized) return;
    k_timer_init(&timer, TimerCallback, NULL);
    k_work_init(&work, WorkCallback);
    timer.user_data = &work;
}

void Timer::Start(uint32_t ms) {
    if(!initialized) Init();
    k_timer_start(&timer, K_MSEC(ms), K_MSEC(ms));
}

void Timer::Start(uint32_t duration_ms, uint32_t period_ms) {
    k_timer_start(&timer, K_MSEC(duration_ms), K_MSEC(period_ms));
}

void Timer::Stop() {
    k_timer_stop(&timer);
}