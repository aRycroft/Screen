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
	virtual void addAudioBuffer(juce::AudioSampleBuffer newBuffer) = 0;
	virtual juce::AudioSampleBuffer* getAudioSampleBuffer(int index) = 0;
	virtual void addAudioFile(AudioFile newAudioFile) = 0;
};
