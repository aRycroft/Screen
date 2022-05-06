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
		: DraggableComponent(vTree, 160),
		paramTree(vTree)
	{
		this->addAndMakeVisible(frequencySlider);
		frequencySlider.setRange(0.1, 50.0);
		frequencySlider.setTextValueSuffix(" Hz");
		frequencySlider.setSliderSnapsToMousePosition(false);
		frequencySlider.setInterceptsMouseClicks(false, false);
		frequencySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
		frequencySlider.addListener(this);
	}

	void paintOverChildren(juce::Graphics& g) override
	{
		g.setColour(juce::Colours::black);
		g.drawEllipse(getLocalBounds().reduced(1).toFloat(), 2.0f);

		g.setColour(juce::Colours::green);
		g.fillEllipse(getInnerCircleBounds().reduced(5).toFloat());

		if (selected)
			g.setColour(juce::Colours::darkcyan);
		else
			g.setColour(juce::Colours::black);
		g.fillEllipse(getInnerCircleBounds().reduced(innerCircleReduction).toFloat());
		g.setColour(juce::Colours::white);
		g.drawText(paramTree[Ids::frequency].toString(), getInnerCircleBounds(), juce::Justification::centred);
		g.setColour(juce::Colours::green);

		if (mouseIn) {
			auto pointToDraw = getPointOnEdge();
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
		int radius = getInnerCircleBounds().getWidth() / 2;
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
		if (!pointIsInCircle(point.x, point.y, getInnerCircleBounds().reduced(5).getWidth() / 2 - innerCircleReduction))
		{
			mouseIn = true;
		}
	}

	void mouseMove(const juce::MouseEvent& event) override
	{
		auto point = event.getPosition();
		if (!pointIsInCircle(point.x, point.y, getInnerCircleBounds().reduced(5).getWidth() / 2 - innerCircleReduction))
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
		return !pointIsInCircle(point.x, point.y, getInnerCircleBounds().getWidth() / 2 - innerCircleReduction);
	}

	bool connectionDrag{ false };

	bool pointIsInCircle(int x, int y, int radius)
	{
		int centreX, centreY;
		centreX = centreY = getLocalBounds().getWidth() / 2;
		return juce::square(x - centreX) + juce::square(y - centreY) < juce::square(radius);
	}

	juce::Point<float> getPointOnEdge()
	{
		auto mousePoint = getMouseXYRelative().toFloat();
		auto centre = getInnerCircleBounds().getCentre().toFloat();
		auto radius = getInnerCircleBounds().getWidth() / 2.0 - 7.5;
		auto angle = centre.getAngleToPoint(mousePoint);
		auto line = juce::Line<float>::fromStartAndAngle(centre, radius, angle);
		return line.getEnd();
	}
private:
	juce::ValueTree paramTree;
	juce::Slider frequencySlider;
	int innerCircleReduction = 10;
	bool mouseIn = false;

	juce::Rectangle<int> getInnerCircleBounds()
	{
		auto bounds = getLocalBounds();
		auto width = bounds.getWidth();
		auto height = bounds.getHeight();
		auto topLeft = bounds.getCentre();
		topLeft.addXY(-DEFAULTSIZE / 2, -DEFAULTSIZE / 2);
		auto bottomRight = bounds.getCentre();
		bottomRight.addXY(DEFAULTSIZE / 2, DEFAULTSIZE / 2);
		return juce::Rectangle<int>{topLeft, bottomRight};
	}
};
