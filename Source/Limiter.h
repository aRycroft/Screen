/*
  ==============================================================================

    Limiter.h
    Created: 28 Mar 2020 11:32:15am
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayBuffer.h"

class Limiter 
{
public:
    Limiter();
    float calculateSample(float samp, float lt, float at, float rt);
private:
    DelayBuffer buffer;
    float gain;
    float xPeak;
    float coeff;
};