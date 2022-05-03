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
    virtual void addSoundToGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) = 0;
    virtual void removeSoundFromGrainGenerator(int grainGenIndex, int audioFileIndex, int audioBufferIndex) = 0;
    virtual void setConnectionWeights(int generatorThatMoved) = 0;
};
