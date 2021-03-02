/*
  ==============================================================================

    AudioFile.h
    Created: 12 Sep 2020 4:41:27pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AmplitudeEnvelope.h"

class MyAudioBuffer
{
public:
    MyAudioBuffer(juce::AudioBuffer<float>* audio, int lowSample, int highSample)
    {
        this->audio = audio;
        this->lowSample = lowSample;
        this->highSample = highSample;
    }

    float getSampleAtBoundedIndexWithAmplitudeWindow(int channel, int index) 
    {
        int boundedIndex = index + lowSample;
        if (boundedIndex < 0 || boundedIndex >= highSample)
        {
            return 0.0f;
        }

        float sampleValue = audio->getSample((channel % audio->getNumChannels()), boundedIndex);
        float sampleWithWindow = sampleValue * hammingWindow.getAmplitudeByProportion((float) index / (highSample - lowSample));
        return sampleWithWindow;
    }

    int getNumChannels() 
    {
        return audio->getNumChannels();
    }

    int getMinIndex()
    {
        return lowSample;
    }

    int getMaxIndex()
    {
        return highSample;
    }

private:
    AmplitudeEnvelope hammingWindow;
    juce::AudioBuffer<float>* audio;
    int lowSample, highSample;
};