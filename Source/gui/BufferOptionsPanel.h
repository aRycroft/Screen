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
		addAndMakeVisible(lowSampleSlider);
		addAndMakeVisible(highSampleSlider);
		lowSampleSlider.setTextValueSuffix(" Samples");
		highSampleSlider.setTextValueSuffix(" Samples");
	}

	void paint(juce::Graphics& g) override
	{
        g.setColour(juce::Colours::olive);
		g.fillRect(getLocalBounds());
	}

	void resized() override
	{
		juce::Grid grid;
		using Track = juce::Grid::TrackInfo;
		using Fr = juce::Grid::Fr;
		grid.templateRows = { Track(Fr(1))};
		grid.templateColumns = { Track(Fr(1)), Track(Fr(1)) };
		grid.items = {juce::GridItem(lowSampleSlider), juce::GridItem(highSampleSlider)};
		grid.performLayout(getLocalBounds());
	}

	void setValueTree(juce::ValueTree valueTree)
	{
		bufferValueTree = valueTree;
		lowSampleSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0, bufferValueTree[Ids::maxSample]));
		lowSampleSlider.getValueObject().referTo(bufferValueTree.getPropertyAsValue(Ids::lowSample, nullptr));
		highSampleSlider.setNormalisableRange(juce::NormalisableRange<double>(0.0, bufferValueTree[Ids::maxSample]));
		highSampleSlider.getValueObject().referTo(bufferValueTree.getPropertyAsValue(Ids::highSample, nullptr));
	}
private:
	juce::ValueTree bufferValueTree;
	juce::Slider lowSampleSlider, highSampleSlider;
};