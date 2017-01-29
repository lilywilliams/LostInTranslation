/*
WordsAndMusic.cpp
*/
#include "SoundWave.h"
#include "BufferStream.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <random>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

void squareWaveGenerator(std::deque<double>& buffer, int numOfSamples)
{
    int i;
    for(i=0; i<numOfSamples/2; i++)
        buffer.push_back(30000);
    for(i; i<numOfSamples; i++)
        buffer.push_back(-30000);
}

void randomWaveGenerator(std::deque<double>& buffer, int numOfSamples)
{
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distribution(-0.5, 0.5);

    for (size_t i(0); i<numOfSamples; ++i) {
        buffer.push_back(distribution(generator) * 30000);
    }
}

double simpleFilter(double current, double next, double prev)
{
    return (current+next)*.9997/2;
}

void translater(std::string word, size_t &mean, size_t &mode, size_t &curiosity)
{
    size_t wordLength(word.length());
    size_t letterFrequency[26] = {0};
    mean = 0;
    mode = 0;
    curiosity = 0;

    for (size_t i(0); i<wordLength; ++i)
    {
        if (isalpha(word[i])) {
            if (isupper(word[i])) ++curiosity;
            if (i>0 && word[i]==word[i-1]) ++curiosity;
            if (i>1 && word[i] == word[i-2]) ++curiosity;

            size_t character(tolower(word[i]) - 'a');
            mean += character;
            ++letterFrequency[character];
        }
        else {
            curiosity+=2;
        }
    }
    if (wordLength > 0)
        mean = mean / wordLength;
    else
        mean = 0;

    size_t uniqueLetters(0);
    size_t modeFrequency(0);
    for (size_t i(0); i<26; ++i)
    {
        if (letterFrequency[i] > 0) ++uniqueLetters;
        if (letterFrequency[i] > modeFrequency) {
            mode = i;
            modeFrequency = letterFrequency[i];
        }
    }
    if (uniqueLetters > .65*wordLength) ++curiosity;
    if (uniqueLetters > .8*wordLength) ++curiosity;
}

int main()
{
    //sf::RenderWindow window(sf::VideoMode(200,200), "Words and Music");

    // Create function pointers to use in creating a SoundWave object
    void (*squareWaveGeneratorPtr)(std::deque<double>&, int) = squareWaveGenerator;
    void (*randomWaveGeneratorPtr)(std::deque<double>&, int) = randomWaveGenerator;
    double (*simpleFilterPtr)(double, double, double) = simpleFilter;

    SoundWave waves[48];
    for (size_t i(0); i<48; ++i) {
        double factor(pow(2, i / 12.0));
        waves[i].setFrequency(factor*110.0);
        waves[i].setFilter(simpleFilterPtr);
    }

    BufferStream bStream(1, 44100);

    std::cout << "Welcome to Words and Music - an art piece by Lily Williams\n\n";

    bool keepRunning(true);
    while (keepRunning)
    {
        std::cout << "\nType text into the console and press Enter. A melody will be generated based on what you type.\n";
        // There must be either a more elegant or more efficient way to do this
        std::vector<std::string> words;
        std::string word, sentence;
        getline(std::cin, sentence);
        std::istringstream sstream(sentence);
        while (getline(sstream, word, ' ')) {
            words.push_back(word);
        }

        for (size_t index(0); index<words.size(); ++index)
        {
            word = words[index];

            size_t mean, mode, curiosity;
            translater(word, mean, mode, curiosity);

            size_t noteIndex((mean+mode) % 48);
            size_t amplitudeModifier(1);
            if (curiosity < 2) amplitudeModifier = .8;
            else if (curiosity % 3 == 0) amplitudeModifier = 1.2;
            size_t sampleNumber( (44100/2)/((26-mode)/6) );

            if (curiosity < 4) waves[noteIndex].setGenerator(randomWaveGeneratorPtr);
            else waves[noteIndex].setGenerator(squareWaveGeneratorPtr);

            waves[noteIndex].stop();
            waves[noteIndex].start();
            for(size_t i(0); i<sampleNumber; ++i)
            {
                bStream.queueSample(waves[noteIndex].getCurrentSample() * amplitudeModifier);
                waves[noteIndex].tic();
            }
        }

        bStream.play();
        while (bStream.getStatus() == BufferStream::Playing)
        {
            sf::sleep(sf::seconds(0.1f));
        }

        std::cout << "\nIf you'd like to continue, press Enter\n";
        std::cout << "If you'd like to quit, type 'Q' and press Enter\n";
        std::string quitString;
        getline(std::cin, quitString);
        if (quitString == "Q" || quitString == "q")
            keepRunning = false;
    }

    return 0;
}
