/*
  ==============================================================================

    AudioBufferSelectorVis.h
    Created: 20 Feb 2021 6:29:19pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h" 
class AudioBufferSelectorVis : public DraggableComponent
{
public:
	AudioBufferSelectorVis(int audioFileTreeId, int lowSample, int highSample, int maxSample)
		: DraggableComponent()
	{
		this->audioFileTreeId = audioFileTreeId;
		this->lowSample = lowSample;
		this->highSample = highSample;
		this->maxSample = maxSample;
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRect(getLocalBounds().reduced(1));
	}

	int audioFileTreeId, lowSample, highSample, maxSample;
};
