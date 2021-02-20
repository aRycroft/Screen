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
class AudioFileVis : public DraggableComponent
{
public:
    AudioFileVis(juce::ValueTree vTree)
        : DraggableComponent(vTree),
		paramTree(vTree)
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::white);
		g.fillRect(getLocalBounds().reduced(1));
	}

	void resized() override
	{
	}
private:
	juce::ValueTree paramTree;
};
