/*
  ==============================================================================

    BufferOptionsPanel.h
    Created: 22 Aug 2021 8:46:15am
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class BufferOptionsPanel : public juce::Component
{
public:
    BufferOptionsPanel()
    {
		addAndMakeVisible(distanceSlider);
		distanceSlider.setRange(0.0, 1.0);
		distanceSlider.setTextValueSuffix(" Hz");
	}

	void paint(juce::Graphics& g) override
	{
        g.setColour(juce::Colours::olive);
		g.fillRect(getLocalBounds());
	}

	void resized() override
	{
		distanceSlider.setBounds(0, 0, 200, 100);
	}

	void setValueTree(juce::ValueTree valueTree)
	{
		bufferValueTree = valueTree;
		distanceSlider.getValueObject().referTo(bufferValueTree.getPropertyAsValue(Ids::distance, nullptr));
	}
private:
	juce::ValueTree bufferValueTree;
	juce::Slider distanceSlider;
};