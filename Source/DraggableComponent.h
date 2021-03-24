/*
  ==============================================================================

	DraggableComponent.h
	Created: 8 Nov 2020 2:32:18pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Utils.h"
class DraggableComponent : public juce::Component
{
public:
	DraggableComponent(){}

	DraggableComponent(juce::ValueTree vTree)
		: paramTree(vTree)
	{
		
	}

	void mouseDown(const juce::MouseEvent& e) override
	{
		mouseDownWithinTarget = e.getEventRelativeTo(this).getMouseDownPosition();
	}

	void mouseUp(const juce::MouseEvent& e) override
	{
		this->getParentComponent()->mouseUp(e);
	}

	void mouseDrag(const juce::MouseEvent& e) override
	{
		auto bounds = getBounds();
		bounds += e.getEventRelativeTo(this).getPosition() - mouseDownWithinTarget;

		auto x = bounds.getX();
		x = std::max(x, 0);
		x = std::min(x, getParentWidth() - getWidth());

		auto y = bounds.getY();
		y = std::max(y, 0);
		y = std::min(y, getParentHeight() - getHeight());

		bounds.setX(x);
		bounds.setY(y);
		setBounds(bounds);
		if (paramTree.isValid()) {
			paramTree.setProperty(Ids::x, (float)bounds.getX() / (getParentWidth() - getWidth()), nullptr);
			paramTree.setProperty(Ids::y, (float)bounds.getY() / (getParentHeight() - getHeight()), nullptr);
		}
		/*This is inefficient but it makes components move smoothly so who cares*/
		getParentComponent()->repaint();
	}

	juce::Rectangle<float> calculateBounds(float size = 60)
	{
		if (paramTree.isValid()) {
			return  juce::Rectangle<float>{ (float)paramTree.getProperty(Ids::x)* (this->getParentWidth() - getWidth()),
				(float)paramTree.getProperty(Ids::y)* (getParentHeight() - getHeight()), size, size };
		}
		return juce::Rectangle<float>();
	}
	bool selected{ false };
protected:
	juce::Point<int> mouseDownWithinTarget;
private:
	juce::ValueTree paramTree;
};
