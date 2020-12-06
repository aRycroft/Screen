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
		:
		DraggableComponent(vTree),
		paramTree(vTree)
	{
	}

	void paint(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 100.0f);
	}

	void resized() override
	{
	}

	juce::Rectangle<float> calculateBounds() 
	{
		return  juce::Rectangle<float>{ (float)paramTree.getProperty(Ids::x) * (this->getParentWidth() - getWidth()),
			(float)paramTree.getProperty(Ids::y) * (getParentHeight() - getHeight()), 60, 60 };
	}

	juce::ValueTree getValueTree() 
	{
		return paramTree;
	}

private:
	juce::ValueTree paramTree;
};
