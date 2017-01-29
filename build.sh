#!/bin/bash
g++ -c -std=c++11 WordsAndMusic.cpp
g++ -c BufferStream.cpp
g++ -c SoundWave.cpp
g++ -o WordsAndMusic WordsAndMusic.o BufferStream.o SoundWave.o -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
