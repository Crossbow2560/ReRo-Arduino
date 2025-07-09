#ifndef ENCODER_TIMER_H
#define ENCODER_TIMER_H

#include <Arduino.h>
#include "TimerOne.h"

class EncoderTimer {
public:
    EncoderTimer(uint8_t pinA, uint8_t pinB, unsigned long pollInterval_us=2000);

    void begin();
    void reset();
    long getCount();
    float getSpeedRpm(unsigned int ppr); // ppr: pulses per revolution
    unsigned long getLastUpdateTime();

private:
    static void isr();
    static volatile long count;
    static uint8_t encoderPinA;
    static uint8_t encoderPinB;
    static int lastEncoded;
    static unsigned long lastTime;
    static volatile unsigned long lastUpdateTime;
    static unsigned long intervalUs;
    static void update();
};

#endif
