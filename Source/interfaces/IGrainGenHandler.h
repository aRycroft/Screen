/*
  ==============================================================================

    IGrainGenHandler.h
    Created: 6 Dec 2020 4:34:51pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class IGrainGenHandler
{
public:
    virtual void createGrainGenerator(juce::ValueTree generatorValueTree) = 0;
    virtual void removeGrainGenerator(int indexToRemove) = 0;
    virtual void setFrequency(int nodeId, float frequency) = 0;
    virtual void generatorMoved(juce::ValueTree generatorThatMoved) = 0;
};
