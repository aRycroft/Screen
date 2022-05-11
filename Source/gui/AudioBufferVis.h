/*
  ==============================================================================

	AudioFileVis.h
	Created: 8 Nov 2020 2:37:31pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h"
class AudioBufferVis : public DraggableComponent, public juce::ChangeListener, public juce::Value::Listener
{
public:
	AudioBufferVis(juce::ValueTree vTree)
		: DraggableComponent(vTree, 60)
	{
		lowSample = paramTree.getPropertyAsValue(Ids::lowSample, nullptr);
		highSample = paramTree.getPropertyAsValue(Ids::highSample, nullptr);
		lowSample.addListener(this);
		highSample.addListener(this);
	}

	~AudioBufferVis()
	{
		thumbnail->removeChangeListener(this);
	}

	void paint(juce::Graphics& g) override
	{
		if (this->selected)
		{
			g.setColour(juce::Colours::darkcyan);
		}
		else
		{
			g.setColour(juce::Colours::red);
		}
		g.fillRect(getLocalBounds().reduced(1));
		g.setColour(juce::Colours::white);
		if (thumbnail != nullptr) 
		{
			thumbnail->drawChannels(g,
				getLocalBounds(),
				(int) lowSample.getValue() / DUMMYSAMPLERATE,
				(int) highSample.getValue() / DUMMYSAMPLERATE,
				1.0f);
		}
	}

	void changeListenerCallback(juce::ChangeBroadcaster* source) override
	{
		repaint();
	}

	void valueChanged(Value& value) override
	{
		repaint();
	};


	void setThumbnail(juce::AudioThumbnail* thumb)
	{
		thumbnail = thumb;
		thumbnail->addChangeListener(this);
	}

private:
	juce::AudioThumbnail* thumbnail{ nullptr };
	juce::Value lowSample, highSample;
};
