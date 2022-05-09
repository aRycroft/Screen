/*
  ==============================================================================

	AudioFile.h
	Created: 12 Sep 2020 4:41:27pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AmplitudeEnvelope.h"
#include "Grain.h"
#include "Utils.h"

class MyAudioBuffer : juce::ValueTree::Listener
{
public:
	MyAudioBuffer(juce::ValueTree valueTree, juce::AudioBuffer<float>* audio, const juce::Value lowSample, const juce::Value highSample)
		: paramTree(valueTree)
	{
		this->audio = audio;
		this->lowSampleValue.referTo(lowSample);
		this->highSampleValue.referTo(highSample);
		paramTree.addListener(this);
	}

	float getSampleAtBoundedIndexWithAmplitudeWindow(int channel, int index, int lowSample, int highSample)
	{
		int boundedIndex = index + (int)lowSample;
		if (boundedIndex < 0 || boundedIndex >= highSample || boundedIndex >= audio->getNumSamples())
		{
			return 0.0f;
		}

		float sampleValue = audio->getSample((channel % audio->getNumChannels()), boundedIndex);
		float sampleWithWindow = sampleValue * hammingWindow.getAmplitudeByProportion((float)index / (highSample - lowSample));
		return sampleWithWindow;
	}

	int getNumChannels()
	{
		return audio->getNumChannels();
	}

	int getMinIndex()
	{
		return (int)lowSampleValue.getValue();
	}

	int getMaxIndex()
	{
		return (int)highSampleValue.getValue();
	}

	void fillNextBuffer(juce::AudioBuffer<float>* outputBuffer)
	{
		for (auto grain : grains)
		{
			if (grain->isGrainPlaying)
			{
				auto currentSample = grain->currentSample;
				auto highSample = grain->highSample;
				auto lowSample = grain->lowSample;
				if (getMaxIndex() < grain->currentSample + getMinIndex())
				{
					grain->isGrainPlaying = false;
				}

				for (int channelIndex{ 0 }; channelIndex < outputBuffer->getNumChannels(); channelIndex++)
				{
					for (int i{ 0 }; i < outputBuffer->getNumSamples(); i++)
					{
						outputBuffer->setSample(channelIndex, i, outputBuffer->getSample(channelIndex, i) +
							getSampleAtBoundedIndexWithAmplitudeWindow(channelIndex, currentSample + i, lowSample, highSample));
					}
				}
				grain->currentSample += outputBuffer->getNumSamples();
			}

		}
	}

	void playGrain()
	{
		for (auto grain : grains)
		{
			if (!grain->isPlaying())
			{
				grain->startPlaying(lowSampleValue.getValue(), highSampleValue.getValue());
				break;
			}
		}
	}

	void valueTreePropertyChanged(juce::ValueTree& vTree, const juce::Identifier& property) override
	{
		if (property == Ids::numVoices)
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
		grains.add(std::make_unique<Grain>(paramTree.getPropertyAsValue(Ids::jitter, nullptr)));
		numVoices++;
	}

	void removeGrainVoice()
	{
		if (!grains.isEmpty())
		{
			grains.removeLast(1);
			numVoices--;
		}
	}
	void initParamTreeValues()
	{
		for (int i = 0; i < paramTree.getNumProperties(); i++)
		{
			paramTree.sendPropertyChangeMessage(paramTree.getPropertyName(i));
		}
	}
private:
	juce::ValueTree paramTree;
	AmplitudeEnvelope hammingWindow;
	juce::OwnedArray<Grain> grains;
	juce::AudioBuffer<float>* audio;
	juce::Value lowSampleValue, highSampleValue;
	int numVoices{ 0 };

};