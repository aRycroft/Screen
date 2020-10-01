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
#include "AudioFile.h"

class Grain
{
public:
	Grain(juce::Array<AudioFile*>* sounds, int startSample, int endSample)
	{
		activeSounds = sounds;
		setStartAndEndSample(startSample, endSample);
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		bool allSoundsFinished{ true };
		for (auto* currentSound : *activeSounds) {
			int dbgMaxIndex = currentSound->getMaxIndex();
			int dbgMinIndex = currentSound->getMinIndex();
			if (currentSound->getMaxIndex() - currentSound->getMinIndex() > currentSample) {
				allSoundsFinished = false;
			}
			for (int i{ 0 }; i < outputBuffer->getNumSamples() && currentSample < endSample; i++, currentSample++) {
				for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++) {
					outputBuffer->setSample(channelIndex, i,
						outputBuffer->getSample(channelIndex, i) +
						currentSound->getSampleAtBoundedIndex(channelIndex, currentSample));
				}
			}
		}
		if (allSoundsFinished || currentSample >= endSample) {
			isGrainPlaying = false;
		}
	}

	void startPlaying(int startSample, int endSample)
	{
		setStartAndEndSample(startSample, endSample);
		isGrainPlaying = true;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}

	void setStartAndEndSample(int startSample, int endSample)
	{
		this->startSample = startSample;
		this->endSample = endSample;
		currentSample = startSample;
	}
private:
	bool isGrainPlaying{ false };
	int startSample, endSample, currentSample;
	juce::Array<AudioFile*>* activeSounds;
};
