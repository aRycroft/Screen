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
#include "Utils.h"

class GrainGenerator : public juce::ValueTree::Listener
{
public:
	GrainGenerator(int sampleRate, juce::ValueTree vTree)
		: paramTree(vTree)
	{
		this->sampleRate = sampleRate;
		paramTree.addListener(this);
	}

	void initParamTreeValues()
	{
		for (int i = 0; i < paramTree.getNumProperties(); i++)
		{
			paramTree.sendPropertyChangeMessage(paramTree.getPropertyName(i));
		}
	}

	bool shouldPlayGrain()
	{
		if (!isActive) return false;

		if (counter++ % 100 == 0) {
			return true;
		}
		return false;
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		for (auto grain : grains2) {
			if (grain->isPlaying()) {
				grain->fillNextBuffer(outputBuffer);
			}
		}
	}

	void playGrain()
	{
		for (auto sound : activeSounds) {
			for (auto grain : grains2) {
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
	juce::OwnedArray<Grain> grains2;
	int sampleRate;
	int numVoices{ 0 };
	unsigned counter{ 0 };

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
		grains2.add(std::make_unique<Grain>());
		numVoices++;
	}

	void removeGrainVoice()
	{
		if (!grains2.isEmpty())
		{
			grains2.removeLast(1);
			numVoices--;
		}
	}
};

