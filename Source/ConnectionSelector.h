/*
  ==============================================================================

    ConnectionSelector.h
    Created: 14 Mar 2021 6:55:26pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class ConnectionSelector : public juce::Component
{
public:
	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::green);
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 100.0f);
	}

	void resized() override
	{
	}
};
