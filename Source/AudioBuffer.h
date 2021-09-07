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
    MyAudioBuffer(juce::AudioBuffer<float>* audio, const juce::Value lowSample, const juce::Value highSample)
    {
        this->audio = audio;
        this->lowSampleValue.referTo(lowSample);
        this->highSampleValue.referTo(highSample);
    }

    float getSampleAtBoundedIndexWithAmplitudeWindow(int channel, int index, int jitter) 
    {
        int lowSample = (int)lowSampleValue.getValue() + jitter;
        int highSample = (int)highSampleValue.getValue() + jitter;
        int boundedIndex = index + (int) lowSample;
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
        return (int)lowSampleValue.getValue();
    }

    int getMaxIndex()
    {
        return (int)highSampleValue.getValue();
    }
private:
    AmplitudeEnvelope hammingWindow;
    juce::AudioBuffer<float>* audio;
    juce::Value lowSampleValue, highSampleValue;
};