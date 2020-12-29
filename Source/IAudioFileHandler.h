/*
  ==============================================================================

    IAudioFileHandler.h
    Created: 6 Dec 2020 4:39:59pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class IAudioFileHandler
{
public:
	virtual void addAudioBuffer(juce::ValueTree newAudioSource) = 0;
	virtual void addAudioFile(juce::ValueTree audioSource, juce::ValueTree childOfSource) = 0;
};
