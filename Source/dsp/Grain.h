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
		if (grainSound->getMaxIndex() < currentSample + grainSound->getMinIndex()) 
		{
			isGrainPlaying = false;
		}

		for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++) 
		{
			for (int i{ 0 }; i < outputBuffer->getNumSamples(); i++) 
			{
				outputBuffer->setSample(channelIndex, i, outputBuffer->getSample(channelIndex, i) +
					grainSound->getSampleAtBoundedIndexWithAmplitudeWindow(channelIndex, currentSample + i, lowSample, highSample));
			}
		}
		currentSample += outputBuffer->getNumSamples();
	}

	void startPlaying(MyAudioBuffer* bufferToPlay)
	{
		isGrainPlaying = true;
		int jitterVal = jitterValue.getValue();
		if (jitterVal > 0) 
		{
			jitter = rng.nextInt(juce::Range<int>(-jitterVal, jitterVal));
		}
		currentSample = 0;
		lowSample = bufferToPlay->getMinIndex() + jitter;
		highSample = bufferToPlay->getMaxIndex() + jitter;
		grainSound = bufferToPlay;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}

private:
	bool isGrainPlaying{ false };
	int currentSample{ 0 }, jitter{ 0 }, lowSample{ 0 }, highSample{ 0 };
	MyAudioBuffer* grainSound;
	juce::Value jitterValue;
	juce::Random rng;
};
