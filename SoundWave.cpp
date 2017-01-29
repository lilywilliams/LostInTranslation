/*
SoundWave.cpp
*/

#include "SoundWave.h"
#include <deque>

SoundWave::SoundWave(): frequency(440), sampleRate(44100)
{}

SoundWave::SoundWave(double initFrequency, double initSampleRate):
    frequency(initFrequency), sampleRate(initSampleRate)
{}

SoundWave::SoundWave(double initFrequency, double initSampleRate,
                     void (*initGenerator)(std::deque<double>&, int),
                     double (*initFilter)(double, double, double)):
    frequency(initFrequency), sampleRate(initSampleRate),
    generator(initGenerator), filter(initFilter)
{}

void SoundWave::setFrequency(double initFrequency)
{
    frequency = initFrequency;

    // Instant resizing of the buffer works, but might create artifacts
    // Whenever frequency is changed, the wave is reset
    if (!buffer.empty())
    {
        stop();
        start();
    }
}

void SoundWave::setSampleRate(double initSampleRate)
{
    sampleRate = initSampleRate;

    // Instant modifying of the buffer works, but might create artifacts
    // Whenever samplerate is changed, the wave is reset
    if (!buffer.empty())
    {
        stop();
        start();
    }
}

void SoundWave::setGenerator(void (*initGenerator)(std::deque<double>&, int))
{
    generator = initGenerator;
}

void SoundWave::setFilter(double (*initFilter)(double, double, double))
{
    filter = initFilter;
}

double SoundWave::getFrequency()
{
    return frequency;
}

double SoundWave::getSampleRate()
{
    return sampleRate;
}

void SoundWave::start()
{
    generator(buffer, sampleRate/frequency);
}

void SoundWave::stop()
{
    // There's gotta be a built-in way to do this
    while (!buffer.empty()) {
        buffer.pop_front();
    }
}

void SoundWave::tic()
{
    double current = buffer.front();
    buffer.pop_front();
    double next = buffer.front();
    double prev = buffer.back();
    current = filter(current, next, prev);
    buffer.push_back(current);
}

double SoundWave::getCurrentSample()
{
    return buffer.front();
}
