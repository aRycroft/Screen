/*
  ==============================================================================

    GrainOptionsPanel.h
    Created: 22 Aug 2021 2:59:53pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class GrainOptionsPanel : public juce::Component
{
public:
	GrainOptionsPanel()
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRect(getLocalBounds());
	}

	void resized() override
	{
	}

	void setValueTree(juce::ValueTree valueTree)
	{
		grainValueTree = valueTree;
	}

private:
	juce::ValueTree grainValueTree;
};