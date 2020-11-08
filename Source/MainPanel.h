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

class MainPanel : public juce::Component, public juce::ValueTree::Listener
{
public:
	MainPanel(juce::ValueTree genTree)
		:vTree(genTree)
	{
		vTree.addListener(this);
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

	void valueTreePropertyChanged(juce::ValueTree& vTree, const juce::Identifier& property) override
	{
		if (property == Ids::active) {
			if (vTree.getProperty(property)) {
				addGeneratorVis();
			}
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
	juce::ValueTree vTree;
};
