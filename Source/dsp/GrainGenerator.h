/*
  ==============================================================================

	GrainGenerator.h
	Created: 12 Sep 2020 5:35:53pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>
#include "Grain.h"
#include "AudioBuffer.h"

class GrainGenerator : public juce::ValueTree::Listener
{
public:
	GrainGenerator(juce::ValueTree vTree)
		: paramTree(vTree)
	{
		paramTree.addListener(this);
	}

	void initParamTreeValues()
	{
		for (int i = 0; i < paramTree.getNumProperties(); i++)
		{
			paramTree.sendPropertyChangeMessage(paramTree.getPropertyName(i));
		}
	}

	void addActiveSound(MyAudioBuffer* newAudioBuffer)
	{
		if (newAudioBuffer != nullptr && !activeSounds.contains(newAudioBuffer)) 
		{
			activeSounds.add(newAudioBuffer);
		}
	}

	void removeSound(MyAudioBuffer* bufferToRemove)
	{
		if (bufferToRemove != nullptr && activeSounds.contains(bufferToRemove)) 
		{
			activeSounds.removeFirstMatchingValue(bufferToRemove);
		}
	}

	juce::Array<MyAudioBuffer*> getActiveSounds()
	{
		return activeSounds;
	}
private:
	juce::Array<MyAudioBuffer*> activeSounds;
	juce::ValueTree paramTree;
};

