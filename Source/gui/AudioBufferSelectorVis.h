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
class AudioBufferSelectorVis : public juce::Component
{
public:
	AudioBufferSelectorVis(int audioFileTreeId, int lowSample, int highSample, int maxSample)
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

	void mouseDown(const MouseEvent& e)
	{
		myDragger.startDraggingComponent(this, e);
	}

	void mouseDrag(const MouseEvent& e)
	{
		myDragger.dragComponent(this, e, nullptr);
	}

	int audioFileTreeId, lowSample, highSample, maxSample;
private:
	juce::ComponentDragger myDragger;
};
