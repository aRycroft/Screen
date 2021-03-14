/*
  ==============================================================================

	GrainGeneratorVis.h
	Created: 7 Nov 2020 2:41:26pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h"
#include "ConnectionSelector.h"

class GrainGeneratorVis : public DraggableComponent, public juce::Slider::Listener
{
public:
	GrainGeneratorVis(juce::ValueTree vTree)
		: DraggableComponent(vTree),
		paramTree(vTree)
	{
		this->addAndMakeVisible(frequencySlider);
		frequencySlider.setRange(50, 5000.0);
		frequencySlider.setTextValueSuffix(" Hz");
		frequencySlider.setSliderSnapsToMousePosition(false);
		frequencySlider.setInterceptsMouseClicks(false, false);
		frequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		frequencySlider.addListener(this);

		this->addAndMakeVisible(connectionSelector);
		connectionSelector.setInterceptsMouseClicks(false, false);
	}

	void paint(juce::Graphics& g) override
	{

	}

	void paintOverChildren(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(getLocalBounds().reduced(5).toFloat(), 100.0f);
		g.setColour(juce::Colours::white);
		g.drawText(paramTree[Ids::frequency].toString(), getLocalBounds(), juce::Justification::centred);
	}

	void resized() override
	{
		frequencySlider.setBounds(0, 0, getWidth() * 4, getHeight() * 4);
		connectionSelector.setBounds(getLocalBounds());
	}

	juce::ValueTree getValueTree()
	{
		return paramTree;
	}

	void mouseDown(const juce::MouseEvent& event) override
	{
		if (event.mods.isRightButtonDown())
		{
			frequencySlider.mouseDown(event);
		}
		else if(mouseDownOnEdge(event))
		{
			connectionDrag = true;
		}
		else
		{
			DraggableComponent::mouseDown(event);
		}
	}

	void mouseDrag(const juce::MouseEvent& event) override
	{
		if (event.mods.isRightButtonDown())
		{
			frequencySlider.mouseDrag(event);
		}
		else if (!connectionDrag) 
		{
			DraggableComponent::mouseDrag(event);
		}
	}

	void mouseUp(const juce::MouseEvent& event) override
	{
		connectionDrag = false;
		/*if (event.mods.isRightButtonDown())
		{
			frequencySlider.setInterceptsMouseClicks(false, false);
		}*/
	}

	void sliderValueChanged(juce::Slider* slider) override
	{
		paramTree.setProperty(Ids::frequency, slider->getValue(), nullptr);
	}

	bool mouseDownOnEdge(const juce::MouseEvent& event)
	{
		auto bounds = getLocalBounds();
		auto visBounds = getLocalBounds().reduced(5);
		auto position = event.getPosition();
		return !visBounds.contains(position) && bounds.contains(position);
	}
	bool connectionDrag{ false };
private:
	juce::ValueTree paramTree;
	juce::Slider frequencySlider;
	ConnectionSelector connectionSelector;
};
