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
	}

	void paintOverChildren(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::green);
		g.fillEllipse(getLocalBounds().reduced(5).toFloat());

		if (selected)
			g.setColour(juce::Colours::darkcyan);
		else
			g.setColour(juce::Colours::black);
		g.fillEllipse(getLocalBounds().reduced(innerCircleReduction).toFloat());
		g.setColour(juce::Colours::white);
		g.drawText(paramTree[Ids::frequency].toString(), getLocalBounds(), juce::Justification::centred);
		g.setColour(juce::Colours::green);

		if (mouseIn) {
			auto pointToDraw = util::getPointOnEdge(*this);
			g.fillEllipse(pointToDraw.getX() - 5, pointToDraw.getY() - 5, 10, 10);
		}
	}

	void resized() override
	{
		frequencySlider.setBounds(0, 0, getWidth() * 4, getHeight() * 4);
	}

	juce::ValueTree getValueTree()
	{
		return paramTree;
	}

	void mouseDown(const juce::MouseEvent& event) override
	{
		readyToDrag = false;
		if (event.mods.isRightButtonDown())
		{
			frequencySlider.mouseDown(event);
		}
		else if (mouseIn)
		{
			connectionDrag = true;
		}
		else 
		{
			readyToDrag = true;
		}
	}

	bool hitTest(int x, int y) override
	{
		int radius = getLocalBounds().getWidth() / 2;
		return pointIsInCircle(x, y, radius);
	}

	void mouseDrag(const juce::MouseEvent& event) override
	{
		readyToDrag = false;

		if (event.mods.isRightButtonDown())
		{
			frequencySlider.mouseDrag(event);
		}
		else if (!connectionDrag)
		{
			readyToDrag = true;
		}
	}

	void mouseUp(const juce::MouseEvent& event) override
	{
		connectionDrag = false;
	}

	void mouseEnter(const juce::MouseEvent& event) override
	{
		auto point = event.getPosition();
		if (!pointIsInCircle(point.x, point.y, getLocalBounds().reduced(5).getWidth() / 2 - innerCircleReduction))
		{
			mouseIn = true;
		}
	}

	void mouseMove(const juce::MouseEvent& event) override
	{
		auto point = event.getPosition();
		if (!pointIsInCircle(point.x, point.y, getLocalBounds().reduced(5).getWidth() / 2 - innerCircleReduction))
		{
			mouseIn = true;
		}
		else 
		{
			mouseIn = false;
		}
		repaint();
	}

	void mouseExit(const juce::MouseEvent& event) override
	{
		mouseIn = false;
		repaint();
	}

	void sliderValueChanged(juce::Slider* slider) override
	{
		paramTree.setProperty(Ids::frequency, slider->getValue(), nullptr);
	}

	bool mouseDownOnEdge(const juce::MouseEvent& event)
	{
		auto point = event.getPosition();
		return !pointIsInCircle(point.x, point.y, getLocalBounds().getWidth() / 2 - innerCircleReduction);
	}

	bool connectionDrag{ false };

	bool pointIsInCircle(int x, int y, int radius)
	{
		int centreX, centreY;
		centreX = centreY = getLocalBounds().getWidth() / 2;
		return juce::square(x - centreX) + juce::square(y - centreY) < juce::square(radius);
	}
private:
	juce::ValueTree paramTree;
	juce::Slider frequencySlider;
	int innerCircleReduction = 10;
	bool mouseIn = false;
};
