/*
  ==============================================================================

    FileListener.h
    Created: 6 Dec 2020 4:31:31pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IAudioFileHandler.h"
#include "AudioFile.h"
class FileListener : public juce::ValueTree::Listener
{
public:
	FileListener(IAudioFileHandler* processor, juce::ValueTree tree)
		: vTree(tree)
	{
		proc = processor;
		vTree.addListener(this);
	};

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		if (parentTree == vTree) {
			proc->addAudioBuffer(childWhichHasBeenAdded);
		}
		else if (parentTree.isAChildOf(vTree)) {
			proc->addAudioFile(parentTree, childWhichHasBeenAdded);
		}
	}
private:
	IAudioFileHandler* proc;
	juce::ValueTree vTree;
};
