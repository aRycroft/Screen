/*
  ==============================================================================

    DelayBuffer.h
    Created: 27 Mar 2020 1:02:36pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DelayBuffer
{
public:
    DelayBuffer();
    DelayBuffer(int bufferSize, int delayOffset);
    void setDelayLength(int length);
    float getData();
    void setData(float data);
    void nextSample();
private:
    juce::AudioSampleBuffer buffer;
    int writeIndex;
    int readIndex;
    int delayLength;
};
