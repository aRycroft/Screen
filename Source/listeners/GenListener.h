/*
  ==============================================================================

    GenListener.h
    Created: 6 Dec 2020 4:31:49pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <IGrainGenHandler.h>
#include <Utils.h>

class GenListener : public juce::ValueTree::Listener
{
public:
	GenListener(IGrainGenHandler* handler, juce::ValueTree tree)
		: grainGenTree(tree)
	{
		_handler = handler;
		grainGenTree.addListener(this);
	};

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
	{
		if (property == Ids::x || property == Ids::y || property == Ids::distance)
		{
			_handler->generatorMoved(treeWhosePropertyHasChanged);
		}

		else if (property == Ids::frequency)
		{
			_handler->setFrequency(grainGenTree.indexOf(treeWhosePropertyHasChanged), treeWhosePropertyHasChanged[property]);
		}
	}

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		_handler->createGrainGenerator(childWhichHasBeenAdded);
	};

	void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
	{
		_handler->removeGrainGenerator(indexFromWhichChildWasRemoved);
	};

private:
	IGrainGenHandler* _handler;
	juce::ValueTree grainGenTree;
};
