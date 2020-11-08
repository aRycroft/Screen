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
		//g.fillAll(juce::Colours::antiquewhite);
	}

	void resized() override
	{
		for (int i{ 0 }; i < NUM_NODES; i++) {
			auto generatorTree = vTree.getChild(i);
			if(generatorTree.getProperty(Ids::active))
				generatorVis[i]->setBounds(
					(float) generatorTree.getProperty(Ids::x) * (getWidth() - generatorVis[i]->getWidth()),
					(float) generatorTree.getProperty(Ids::y) * (getHeight() - generatorVis[i]->getHeight()), 60, 60);
		}
	}

	void valueTreePropertyChanged(juce::ValueTree& vTree, const juce::Identifier& property) override
	{
		auto genNumber = vTree.getType().toString().getLastCharacter() - '0';
		if (property == Ids::active) {
			if (vTree.getProperty(property)) {
				addGeneratorVis(genNumber);
			}
		}
	}

	void addGeneratorVis(int childNumber)
	{
		generatorVis[childNumber].reset(new GrainGeneratorVis(vTree.getChild(childNumber)));
		addAndMakeVisible(*generatorVis[childNumber]);
	}

	void removeGeneratorVis()
	{
	}
private:
	std::unique_ptr<GrainGeneratorVis> generatorVis[NUM_NODES];
	//juce::OwnedArray<GrainGeneratorVis> generatorVis;
	juce::ValueTree vTree;
};
