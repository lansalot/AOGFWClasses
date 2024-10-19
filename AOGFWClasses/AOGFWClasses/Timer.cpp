// 
// 
// 

#include "Timer.h"

void ElapsedTimer::update() {
    unsigned long currentTime = millis();
    m_elapsed += currentTime - m_lastTime;
    m_lastTime = currentTime;
}

unsigned long ElapsedTimer::getElapsed() {
    update();
    return m_elapsed;
}

unsigned long ElapsedTimer::getLastTime() {
    update();
    return m_lastTime;
}

void ElapsedTimer::checkpoint() {
    m_checkpoint = millis();
}
unsigned long ElapsedTimer::getCheckpoint() {
    return millis() - m_checkpoint;
}

void ElapsedTimer::reset() {
    m_checkpoint, m_lastTime = millis();
    m_elapsed = 0;
}