/*
  ==============================================================================

	GrainGeneratorVis.h
	Created: 7 Nov 2020 2:41:26pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GrainGeneratorVis : public juce::Component
{
public:
	GrainGeneratorVis()
		: activeParamListener(this),
		grainVoiceParamListener(this)
	{
	}

	void paint(juce::Graphics& g) override
	{
		if (isVisible())
			DBG("True");
		else
			DBG("False");
		g.fillAll(juce::Colours::lightblue);
	}

	void resized() override
	{
	}

	juce::AudioProcessorValueTreeState::Listener* getActiveParamListener()
	{
		return &activeParamListener;
	}

	juce::AudioProcessorValueTreeState::Listener* getGrainVoiceParamListener()
	{
		return &grainVoiceParamListener;
	}

	bool isActive{ false };

private:
	class ActiveParamListener : public juce::AudioProcessorValueTreeState::Listener
	{
	public:
		ActiveParamListener(GrainGeneratorVis* processor)
		{
			proc = processor;
		};

		void parameterChanged(const juce::String& parameterID, float newValue) override
		{
			if (newValue) {
				proc->setVisible(true);
			}
			else {
				proc->setVisible(false);
			}
		}

	private:
		GrainGeneratorVis* proc;
	};

	class GrainVoiceParamListener : public juce::AudioProcessorValueTreeState::Listener
	{
	public:
		GrainVoiceParamListener(GrainGeneratorVis* gen)
		{
			generator = gen;
		};

		void parameterChanged(const juce::String& parameterID, float newValue) override
		{

		}
	private:
		GrainGeneratorVis* generator;
	};

	ActiveParamListener activeParamListener;
	GrainVoiceParamListener grainVoiceParamListener;
};
