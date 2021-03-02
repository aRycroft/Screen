/*
  ==============================================================================

	Grain.h
	Created: 11 Sep 2020 8:31:38pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioBuffer.h"

class Grain
{
public:
	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		if (grainSound->getMaxIndex() < currentSample + grainSound->getMinIndex()) 
		{
			isGrainPlaying = false;
		}

		for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++) 
		{
			for (int i{ 0 }; i < outputBuffer->getNumSamples(); i++) 
			{
				outputBuffer->setSample(channelIndex, i,
					outputBuffer->getSample(channelIndex, i) +
					grainSound->getSampleAtBoundedIndexWithAmplitudeWindow(channelIndex, currentSample + i));
			}
		}
		currentSample += outputBuffer->getNumSamples();
	}

	void startPlaying(MyAudioBuffer* bufferToPlay)
	{
		isGrainPlaying = true;
		currentSample = 0;
		grainSound = bufferToPlay;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}

private:
	bool isGrainPlaying{ false };
	int currentSample{ 0 };
	MyAudioBuffer* grainSound;
};
