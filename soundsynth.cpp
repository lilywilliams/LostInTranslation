/*
soundsynth.cpp
*/
#include "SoundWave.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

void squareWaveGenerator(std::deque<double>& buffer, int numOfSamples)
{
    int i;
    for(i=0; i<numOfSamples/2; i++)
        buffer.push_back(30000);
    for(i; i<numOfSamples; i++)
        buffer.push_back(-30000);
}

double simpleFilter(double current, double next, double prev)
{
    return (current+next)*.9997/2;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(200,200), "SFML Works?");

    // Create function pointers to use in creating a SoundWave object
    void (*squareWaveGeneratorPtr)(std::deque<double>&, int) = squareWaveGenerator;
    double (*simpleFilterPtr)(double, double, double) = simpleFilter;

    //Test soundwave and sfml Audio
    SoundWave wave(800, 44100, squareWaveGeneratorPtr, simpleFilterPtr);
    wave.start();

    std::vector<sf::Int16> samples(44100, 0);

    for(int i=0; i<44100; i++)
    {
        samples[i] = wave.getCurrentSample();
        wave.tic();
    }

    sf::SoundBuffer buff;
    buff.loadFromSamples(&samples[0], samples.size(), 1, 44100);

    sf::Sound sound;
    sound.setBuffer(buff);
    sound.play();

    // sf::Music music;
    // if(!music.openFromFile("donson3.wav"))
    //     return -1;
    // music.play();

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
