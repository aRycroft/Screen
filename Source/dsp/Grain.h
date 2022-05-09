/*
  ==============================================================================

	Grain.h
	Created: 11 Sep 2020 8:31:38pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Grain
{
public:
	Grain(juce::Value jitter)
	{
		this->jitterValue.referTo(jitter);
	}

	void startPlaying(int lowSample, int highSample)
	{
		isGrainPlaying = true;
		int jitterVal = jitterValue.getValue();
		if (jitterVal > 0) 
		{
			jitter = rng.nextInt(juce::Range<int>(-jitterVal, jitterVal));
		}
		currentSample = 0;
		this->lowSample = lowSample + jitter;
		this->highSample = highSample + jitter;
	}

	bool isPlaying()
	{
		return isGrainPlaying;
	}

	int getCurrentSample()
	{
		return currentSample;
	}
	int currentSample{ 0 }, lowSample{ 0 }, highSample{ 0 };
	bool isGrainPlaying{ false };
private:
	int jitter{ 0 };
	juce::Value jitterValue;
	juce::Random rng;
};
