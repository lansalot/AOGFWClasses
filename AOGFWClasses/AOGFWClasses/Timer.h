// Timer.h

#ifndef _TIMER_h
#define _TIMER_h
#include <Arduino.h>

class ElapsedTimer {

private:
    unsigned long m_lastTime;
    unsigned long m_elapsed;
    unsigned long m_checkpoint;

public:
    ElapsedTimer() {
        m_checkpoint, m_lastTime = millis();
        m_elapsed = 0;
    }

    void update();

    unsigned long getElapsed();
    unsigned long getLastTime();
    unsigned long getCheckpoint();
    void checkpoint();

    void reset();
};


#endif

