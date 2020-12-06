/*
  ==============================================================================

    GenListener.h
    Created: 6 Dec 2020 4:31:49pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IGrainGenHandler.h"

class GenListener : public juce::ValueTree::Listener
{
public:
	GenListener(IGrainGenHandler* processor, juce::ValueTree tree)
		: vTree(tree)
	{
		proc = processor;
		vTree.addListener(this);
	};

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		proc->createGrainGenerator(childWhichHasBeenAdded);
	};

	void valueTreeChildRemoved(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override
	{
		proc->removeGrainGenerator(indexFromWhichChildWasRemoved);
	};

private:
	IGrainGenHandler* proc;
	juce::ValueTree vTree;
};
