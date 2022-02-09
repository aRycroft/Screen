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

	DraggableComponent(juce::ValueTree vTree) : paramTree(vTree){}

	void mouseUp(const juce::MouseEvent& e) override
	{
		this->getParentComponent()->mouseUp(e);
	}

	juce::Rectangle<float> calculateBounds(float size = 60)
	{
		if (paramTree.isValid()) 
		{
			return  juce::Rectangle<float>{ 
				(float) paramTree.getProperty(Ids::x) * (getParentWidth() - size),
				(float) paramTree.getProperty(Ids::y)* (getParentHeight() - size), size, size };
		}
		return juce::Rectangle<float>();
	}
	
	juce::ValueTree getValueTree() 
	{
		return paramTree;
	}

	void setValueTree(juce::ValueTree valueTree)
	{
		paramTree = valueTree;
	}

	float getValueTreeProperty(juce::Identifier key) 
	{
		return paramTree[key];
	}

	void setValueTreeProperty(const juce::Identifier &key, const float &value) 
	{
		if (paramTree.isValid())
		{
			paramTree.setProperty(key, value, nullptr);
		}
	}

	bool readyToDrag{ true };

	bool selected{ false };

	juce::Point<int> mouseDownWithinTarget;
private:
	juce::ValueTree paramTree;
};
