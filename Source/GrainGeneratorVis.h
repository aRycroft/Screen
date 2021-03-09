/*
  ==============================================================================

	GrainGeneratorVis.h
	Created: 7 Nov 2020 2:41:26pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h"

class GrainGeneratorVis : public DraggableComponent
{
public:
	GrainGeneratorVis(juce::ValueTree vTree)
		: DraggableComponent(vTree),
		paramTree(vTree)
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 100.0f);
		g.setColour(juce::Colours::white);
		g.drawText(paramTree[Ids::frequency].toString(), getLocalBounds(), juce::Justification::centred);
	}

	void resized() override
	{
	}

	juce::ValueTree getValueTree() 
	{
		return paramTree;
	}

private:
	juce::ValueTree paramTree;
};
