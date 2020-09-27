/*
  ==============================================================================

    AudioFile.h
    Created: 12 Sep 2020 4:41:27pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"

class AudioFile
{
public:
    AudioFile(juce::AudioBuffer<float> audio, int lowSample, int highSample)
    {
        this->audio = audio;
        this->lowSample = lowSample;
        this->highSample = highSample;
    }

    float getSampleAtBoundedIndex(int channel, int index) 
    {
        int boundedIndex = index + lowSample;
        if (boundedIndex < 0 || boundedIndex >= audio.getNumSamples()) return 0.0f;
        return audio.getSample(channel, boundedIndex);
    }

    int getNumChannels() 
    {
        return audio.getNumChannels();
    }

    int getMinIndex()
    {
        return lowSample;
    }

    int getMaxIndex()
    {
        return highSample;
    }

    void fillBufferWithValue(float val)
    {
        util::fillAudioBufferWithValue(&audio, val);
    }

private:
    juce::AudioBuffer<float> audio;
    int lowSample, highSample;
};