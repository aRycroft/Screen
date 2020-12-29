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
};