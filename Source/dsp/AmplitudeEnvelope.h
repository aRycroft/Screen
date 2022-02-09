/*
  ==============================================================================

    AmplitudeEnvelope.h
    Created: 19 Oct 2020 8:24:46pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
constexpr auto NUMSAMPLES = 2048;
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
            envelopeBuffer[i] = 0.54f - 0.46f * cosf(juce::MathConstants<float>::twoPi * ((float)i / NUMSAMPLES));
        }
    };

    float getAmplitudeValueFromCurrentSample(int currentSample, int samplesInFile) 
    {
        float proportionOfFilePlayed = (float)currentSample / samplesInFile;
        float approxEnvelopeIndex = proportionOfFilePlayed * NUMSAMPLES;
        return envelopeBuffer[(int)floor(approxEnvelopeIndex)];
    }

    float getAmplitudeByProportion(float proportionOfSamplePlayed) {
        int approxIndex = (int) floor(proportionOfSamplePlayed * NUMSAMPLES);
        return envelopeBuffer[approxIndex];
    }

private:
    float envelopeBuffer[NUMSAMPLES];
};
