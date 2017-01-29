/*
BufferStream.cpp
*/

#include "BufferStream.h"
#include <SFML/Audio.hpp>
#include <vector>

bool BufferStream::onGetData(Chunk& data)
{
    clean();

    size_t numOfSamples(mSampleRate < buffer.size() ? mSampleRate : buffer.size());
    //if (numOfSamples == 0) return false;

    data.samples = &buffer[currentSample];
    data.sampleCount = numOfSamples;
    currentSample += numOfSamples;
    return numOfSamples != 0;
}

void BufferStream::onSeek(sf::Time timeOffset)
{
    // Does nothing - this class will not support onSeek.
}

void BufferStream::clean()
{
    std::copy(buffer.begin()+currentSample, buffer.end(), buffer.begin());
    for (size_t i(0); i<currentSample; ++i) {
        buffer.pop_back();
    }
    currentSample = 0;
}

BufferStream::BufferStream() : mSampleRate(44100), currentSample(0)
{
    initialize(1, 44100);
}

BufferStream::BufferStream(unsigned int channelCount, unsigned int sampleRate) :
mSampleRate(sampleRate), currentSample(0)
{
    initialize(channelCount, mSampleRate);
}

void BufferStream::queueSample(sf::Int16 newSample)
{
    buffer.push_back(newSample);
}

void BufferStream::queueSampleVector(std::vector<sf::Int16> newSamples)
{
    buffer.insert(buffer.end(), newSamples.begin(), newSamples.end());
}
