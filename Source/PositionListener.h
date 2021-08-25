/*
  ==============================================================================

	PositionListener.h
	Created: 21 Feb 2021 1:20:52pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IGrainGenHandler.h"
#include "Utils.h"

class PositionListener : public juce::ValueTree::Listener
{
public:
	PositionListener(IGrainGenHandler* handler,
		juce::ValueTree grainGeneratorTree, juce::ValueTree audioFileTree)
		: grainGenTree(grainGeneratorTree), fileTree(audioFileTree)
	{
		_handler = handler;
		grainGenTree.addListener(this);
		fileTree.addListener(this);
	}

	void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
	{
		if (property == Ids::x || property == Ids::y || property == Ids::distance)
		{
			if (treeWhosePropertyHasChanged.hasType(Ids::generator))
			{
				checkAllFileBuffersForRange(treeWhosePropertyHasChanged);
			}
			else if (treeWhosePropertyHasChanged.hasType(Ids::audioBuffer))
			{
				checkAllGrainGeneratorsForRange(treeWhosePropertyHasChanged);
			}
		}
	}

	void checkAllFileBuffersForRange(juce::ValueTree& treeWhosePropertyHasChanged)
	{
		for (juce::ValueTree file : fileTree)
		{
			for (juce::ValueTree buffer : file)
			{
				if (isInRange(buffer[Ids::distance],
					treeWhosePropertyHasChanged[Ids::x], buffer[Ids::x],
					treeWhosePropertyHasChanged[Ids::y], buffer[Ids::y]))
				{
					_handler->addSoundToGrainGenerator(grainGenTree.indexOf(treeWhosePropertyHasChanged), fileTree.indexOf(file), file.indexOf(buffer));
				}
				else
				{
					_handler->removeSoundFromGrainGenerator(grainGenTree.indexOf(treeWhosePropertyHasChanged), fileTree.indexOf(file), file.indexOf(buffer));
				}
			}
		}
	}

	void checkAllGrainGeneratorsForRange(juce::ValueTree& treeWhosePropertyHasChanged) 
	{
		for (juce::ValueTree generator : grainGenTree)
		{
			auto audioFileTree = treeWhosePropertyHasChanged.getParent();
			if (isInRange(treeWhosePropertyHasChanged[Ids::distance], 
				treeWhosePropertyHasChanged[Ids::x], generator[Ids::x],
				treeWhosePropertyHasChanged[Ids::y], generator[Ids::y]))
			{
				_handler->addSoundToGrainGenerator(grainGenTree.indexOf(generator), 
					fileTree.indexOf(audioFileTree),
					audioFileTree.indexOf(treeWhosePropertyHasChanged));
			}
			else
			{
				_handler->removeSoundFromGrainGenerator(grainGenTree.indexOf(generator),
					fileTree.indexOf(audioFileTree),
					audioFileTree.indexOf(treeWhosePropertyHasChanged));
			}
		}
	}

private:
	juce::ValueTree grainGenTree, fileTree;
	IGrainGenHandler* _handler;

	bool isInRange(float distance, float x1, float x2, float y1, float y2)
	{
		return std::abs(x1 - x2) < distance && std::abs(y1 - y2) < distance;
	}
};
