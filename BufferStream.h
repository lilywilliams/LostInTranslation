/*
BufferStream.h
*/

#ifndef BUFFERSTREAM_H
#define BUFFERSTREAM_H

#include <SFML/Audio.hpp>
#include <vector>

class BufferStream : public sf::SoundStream
{
private:
    size_t mSampleRate, currentSample;
    std::vector<sf::Int16> buffer;

    virtual bool onGetData(Chunk& data);
    virtual void onSeek(sf::Time timeOffset);
    void clean();
public:
    BufferStream();
    BufferStream(unsigned int channelCount, unsigned int sampleRate);

    void queueSample(sf::Int16 newSample);
    void queueSampleVector(std::vector<sf::Int16> newSamples);
};

#endif
