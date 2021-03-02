/*
  ==============================================================================

	GrainGenerator.h
	Created: 12 Sep 2020 5:35:53pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Grain.h"
#include "Utils.h"
#include <vector>

class GrainGenerator : public juce::ValueTree::Listener
{
public:
	GrainGenerator(int sampleRate, juce::ValueTree vTree)
		: paramTree(vTree)
	{
		this->sampleRate = sampleRate;
		paramTree.addListener(this);
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		for (auto grain : grains) {
			if (grain->isPlaying()) {
				grain->fillNextBuffer(outputBuffer);
			}
		}
	}

	void playGrain()
	{
		for (auto sound : activeSounds) {
			for (auto grain : grains) {
				if (!grain->isPlaying()) {
					grain->startPlaying(sound);
					break;
				}
			}
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

	bool isActive{ true };

private:
	juce::ValueTree paramTree;
	juce::Array<MyAudioBuffer*> activeSounds;
	std::vector<Grain*> grains;
	int sampleRate;
	int numVoices{ 0 };

	void valueTreePropertyChanged(juce::ValueTree& vTree, const juce::Identifier& property) override
	{
		if (property == Ids::active) {
			isActive = vTree.getProperty(property);
		}
		else if (property == Ids::numVoices)
		{
			int targetNumberofVoices = vTree.getProperty(property);
			int currentNumberofVoices = numVoices;

			while (currentNumberofVoices < targetNumberofVoices)
			{
				addGrainVoice();
				currentNumberofVoices++;
			}
			while (currentNumberofVoices > targetNumberofVoices)
			{
				removeGrainVoice();
				currentNumberofVoices--;
			}
		}
	}

	void addGrainVoice()
	{
		grains.push_back(new Grain{});
		numVoices++;
	}

	void removeGrainVoice()
	{
		if (!grains.empty())
		{
			grains.pop_back();
			numVoices--;
		}
	}
};

