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
		for (auto grain : grains) {
			if (!grain->isPlaying()) {
				grain->startPlaying(0, 0);
				return;
			}
		}
	}

	void addGrainVoice()
	{
		grains.push_back(new Grain{ &activeSounds});
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

	void addActiveSound(AudioBuffer* newAudioFile)
	{
		if (newAudioFile != nullptr && !activeSounds.contains(newAudioFile)) {
			activeSounds.add(newAudioFile);
		}
	}

	void removeSound(AudioBuffer* soundToRemove)
	{
		if (soundToRemove != nullptr && activeSounds.contains(soundToRemove)) {
			activeSounds.removeAllInstancesOf(soundToRemove);
		}
	}

	int getNumVoices()
	{
		return numVoices;
	}

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
	bool isActive{ true };
private:
	juce::ValueTree paramTree;
	juce::Array<AudioBuffer*> activeSounds;
	std::vector<Grain*> grains;
	int sampleRate, numVoices = 0;
};

