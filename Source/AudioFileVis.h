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
		g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 10.0f);
	}

	void resized() override
	{
	}
private:
	juce::ValueTree paramTree;
};
