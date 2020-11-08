/*
  ==============================================================================

	MainPanel.h
	Created: 8 Nov 2020 9:20:51am
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>>
#include "GrainGeneratorVis.h"

class MainPanel : public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
public:
	MainPanel()
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.fillAll(juce::Colours::antiquewhite);
	}

	void resized() override
	{
		int i = 0;
		for each (auto genVis in generatorVis)
		{
			genVis->setBounds(i++ * 100, 0, 50, 50);
		}
	}

	void parameterChanged(const juce::String& parameterID, float newValue) override
	{
		if (newValue)
		{
			addGeneratorVis();
		}
		else
		{
		}
	}

	void addGeneratorVis()
	{
		generatorVis.add(std::unique_ptr<GrainGeneratorVis>(new GrainGeneratorVis()));
		addAndMakeVisible(generatorVis.getLast());
	}

	void removeGeneratorVis() 
	{

	}
private:
	juce::OwnedArray<GrainGeneratorVis> generatorVis;
};
