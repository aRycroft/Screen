/*
  ==============================================================================

    GrainGenerator.h
    Created: 12 Sep 2020 5:35:53pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Grain.h"
#include <vector>

class GrainGenerator
{
public:
    GrainGenerator(int sampleRate, int numVoices, std::atomic<float>* grainSizeMS)
    {
        for (int i{ 0 }; i < numVoices; i++) {
            grains.push_back(new Grain{ &activeSounds, 0, 0 });
        }
        this->grainSize = grainSizeMS;
        this->sampleRate = sampleRate;
    }

    void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
    {
        for (auto grain : grains) {
            if (grain->isPlaying()) {
                grain->fillNextBuffer(outputBuffer);
            }
        }
    }

    void playGrain()
    {
        for (auto grain : grains) {
            if (!grain->isPlaying()) {
                grain->startPlaying(0, (int)(*grainSize / 1000 * sampleRate));
                return;
            }
        }
    }

    /*Based on parameters set in valuetree*/
    void addGrainVoice()
    {
        grains.push_back(new Grain{ &activeSounds, 0, 0 });
    }

    void addActiveSound(AudioFile* newAudioFile)
    {
        activeSounds.add(newAudioFile);
    }

    void removeSound(AudioFile* soundToRemove)
    {
        activeSounds.removeAllInstancesOf(soundToRemove);
    }

private:
    juce::Array<AudioFile*> activeSounds;
    std::vector<Grain*> grains;
    std::vector<int> activeSoundIndices;
    std::atomic<float>* grainSize;
    int sampleRate;
};