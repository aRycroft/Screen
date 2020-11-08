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
	GrainGeneratorVis(juce::ValueTree vTree)
		:paramTree(vTree)
	{
	}
	
	void mouseDown(const juce::MouseEvent& e) override
	{
		mouseDownWithinTarget = e.getEventRelativeTo(this).getMouseDownPosition();
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

		paramTree.setProperty(Ids::x, (float) bounds.getX() / (getParentWidth() - getWidth()), nullptr);
		paramTree.setProperty(Ids::y, (float) bounds.getY() / (getParentHeight() - getHeight()), nullptr);
		/*This is inefficient but it makes components move smoothly so who cares*/
		getParentComponent()->repaint();
	}

	void paint(juce::Graphics& g) override
	{	
		g.setColour(juce::Colours::black);
		g.fillRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 100.0f);
	}

	void resized() override
	{
	}
private:
	juce::ValueTree paramTree;
	juce::Point<int> mouseDownWithinTarget;
};
