#include "EncoderTimer.h"

volatile long EncoderTimer::count = 0;
uint8_t EncoderTimer::encoderPinA = 0;
uint8_t EncoderTimer::encoderPinB = 0;
int EncoderTimer::lastEncoded = 0;
unsigned long EncoderTimer::lastTime = 0;
volatile unsigned long EncoderTimer::lastUpdateTime = 0;
unsigned long EncoderTimer::intervalUs = 2000;

EncoderTimer::EncoderTimer(uint8_t pinA, uint8_t pinB, unsigned long pollInterval_us)
{
    encoderPinA = pinA;
    encoderPinB = pinB;
    intervalUs = pollInterval_us;
}

void EncoderTimer::begin()
{
    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);
    int MSB = digitalRead(encoderPinA);
    int LSB = digitalRead(encoderPinB);
    lastEncoded = (MSB << 1) | LSB;
    lastTime = micros();
    lastUpdateTime = lastTime;
    Timer1.initialize(intervalUs);
    Timer1.attachInterrupt(isr);
}

void EncoderTimer::reset()
{
    count = 0;
    lastTime = micros();
    lastUpdateTime = lastTime;
}

long EncoderTimer::getCount()
{
    return count;
}

float EncoderTimer::getSpeedRpm(unsigned int ppr)
{
    // Get the number of pulses detected in the last interval
    static long lastCount = 0;
    static unsigned long lastCallTime = 0;
    long currentCount = count;
    unsigned long now = micros();
    if (lastCallTime == 0) {
        lastCallTime = now;
        lastCount = currentCount;
        return 0.0;
    }
    long diff = currentCount - lastCount;
    unsigned long dt = now - lastCallTime;
    lastCount = currentCount;
    lastCallTime = now;
    // pulses per microsecond to revolutions per minute
    float rpm = ((float)diff / (float)ppr) * (60000000.0 / (float)dt);
    return rpm;
}

unsigned long EncoderTimer::getLastUpdateTime()
{
    return lastUpdateTime;
}

void EncoderTimer::isr()
{
    update();
}

void EncoderTimer::update()
{
    int MSB = digitalRead(encoderPinA);
    int LSB = digitalRead(encoderPinB);
    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncoded << 2) | encoded;

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
        count++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
        count--;

    lastEncoded = encoded;
    lastUpdateTime = micros();
}