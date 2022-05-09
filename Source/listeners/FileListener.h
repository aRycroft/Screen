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
#include "AudioBuffer.h"
class FileListener : public juce::ValueTree::Listener
{
public:
	FileListener(IAudioFileHandler* handler, juce::ValueTree tree)
		: fileTree(tree)
	{
		_handler = handler;
		fileTree.addListener(this);
	};

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
	{
		if (treeWhosePropertyHasChanged.isAChildOf(fileTree)) {
			if (property == Ids::x || property == Ids::y)
			{
				_handler->audioBufferMoved(treeWhosePropertyHasChanged);
			}
		}
	}

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		if (parentTree == fileTree) 
		{
			_handler->addAudioFile(childWhichHasBeenAdded);
		}
		else if (parentTree.isAChildOf(fileTree)) 
		{
			_handler->addAudioBuffer(parentTree, childWhichHasBeenAdded);
		}
	}
private:
	IAudioFileHandler* _handler;
	juce::ValueTree fileTree;
};
