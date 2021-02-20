/*
  ==============================================================================

    AudioFileVis.h
    Created: 8 Nov 2020 2:37:31pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h"
class AudioBufferVis : public DraggableComponent
{
public:
    AudioBufferVis(juce::ValueTree vTree)
        : DraggableComponent(vTree),
		paramTree(vTree)
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRect(getLocalBounds().reduced(1));
	}

	void resized() override
	{
	}
private:
	juce::ValueTree paramTree;
};
