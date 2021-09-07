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
	Grain(juce::Value jitter)
	{
		this->jitterValue.referTo(jitter);
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		if ((grainSound->getMaxIndex() + jitter) < currentSample + (grainSound->getMinIndex() + jitter)) 
		{
			isGrainPlaying = false;
		}

		for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++) 
		{
			for (int i{ 0 }; i < outputBuffer->getNumSamples(); i++) 
			{
				outputBuffer->setSample(channelIndex, i,
					outputBuffer->getSample(channelIndex, i) +
					grainSound->getSampleAtBoundedIndexWithAmplitudeWindow(channelIndex, currentSample + i, jitter));
			}
		}
		currentSample += outputBuffer->getNumSamples();
	}

	void startPlaying(MyAudioBuffer* bufferToPlay)
	{
		isGrainPlaying = true;
		int jitterVal = jitterValue.getValue();
		jitter = rng.nextInt(juce::Range<int>(-jitterVal, jitterVal));
		currentSample = 0;
		grainSound = bufferToPlay;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}

private:
	bool isGrainPlaying{ false };
	int currentSample{ 0 }, jitter{ 0 };
	MyAudioBuffer* grainSound;
	juce::Value jitterValue;
	juce::Random rng;
};
