/*
SoundWave.h
TODO: add amplitude, and frequency changing
*/

#ifndef SOUNDWAVE_H
#define SOUNDWAVE_H

#include <deque>

class SoundWave
{
private:
    double frequency, sampleRate;
    void (*generator)(std::deque<double>&, int);
    double (*filter)(double, double, double);
    std::deque<double> buffer;

public:
    SoundWave();
    SoundWave(double initFrequency, double initSampleRate);
    SoundWave(double initFrequency, double initSampleRate,
              void (*initGenerator)(std::deque<double>&, int),
              double (*initFilter)(double, double, double));

    void setFrequency(double initFrequency);
    void setSampleRate(double initSampleRate);
    void setGenerator(void (*initGenerator)(std::deque<double>&, int));
    void setFilter(double (*initFilter)(double, double, double));
    double getFrequency();
    double getSampleRate();

    void start();
    void stop();
    void tic();
    double getCurrentSample();
};

#endif
