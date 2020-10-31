/*
  ==============================================================================

    AmplitudeEnvelope.h
    Created: 19 Oct 2020 8:24:46pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#define NUMSAMPLES 2048
#include <JuceHeader.h>

class AmplitudeEnvelope {
    
public:
    AmplitudeEnvelope()
    {
        FillBufferWithHammingValues();
    }

    void FillBufferWithHammingValues() 
    {
        for (int i{ 0 }; i < NUMSAMPLES; i++)
        {
            envelopeBuffer[i] = 0.54 - 0.46 * cosf(juce::MathConstants<float>::twoPi * ((float)i / NUMSAMPLES));
        }
    };

    float getAmplitudeValueFromCurrentSample(int currentSample, int samplesInFile) 
    {
        float proportionOfFilePlayed = (float)currentSample / samplesInFile;
        float approxEnvelopeIndex = proportionOfFilePlayed * NUMSAMPLES;
        return envelopeBuffer[(int)floor(approxEnvelopeIndex)];
    }

private:
    float envelopeBuffer[NUMSAMPLES];
};
