/*
  ==============================================================================

	Grain.h
	Created: 11 Sep 2020 8:31:38pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include <tuple>
#include "Utils.h"
#include "AudioBuffer.h"

class Grain
{
public:
	Grain(juce::Array<AudioBuffer*>* sounds)
	{
		activeSounds = sounds;
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		bool allSoundsFinished{ true };
		for (auto* currentSound : *activeSounds) {
			if (currentSound->getMaxIndex() > currentSample) {
				allSoundsFinished = false;
			}
			for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++) {
				for (int i{ 0 }; i < outputBuffer->getNumSamples(); i++) {
					outputBuffer->setSample(channelIndex, i, 
						outputBuffer->getSample(channelIndex, i) + 
							currentSound->getSampleAtBoundedIndex(channelIndex, currentSample + i));
				}
			}
		}
		currentSample += outputBuffer->getNumSamples();
		if (allSoundsFinished) {
			isGrainPlaying = false;
		}
	}

	void startPlaying(int startSample, int endSample)
	{
		isGrainPlaying = true;
		currentSample = 0;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}
private:
	bool isGrainPlaying{ false };
	int startSample, endSample, currentSample;
	juce::Array<AudioBuffer*>* activeSounds;
};
