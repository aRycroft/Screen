/*
  ==============================================================================

    GroupDragMouseListener.h
    Created: 24 Mar 2021 7:16:54pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DraggableComponent.h"
class GroupDragMouseListener : public juce::MouseListener
{
public:
    GroupDragMouseListener()
    {
            
    }

	void mouseDown(const juce::MouseEvent& e) override
	{
		for (auto component : draggableItemSet)
		{
			if (component != e.eventComponent)
			{
				component->mouseDown(e);
			}
		}
	}

	void mouseDrag(const juce::MouseEvent& e) override
	{
		for (auto component : draggableItemSet) 
		{
			if (component != e.eventComponent) 
			{
				component->mouseDrag(e);
			}
		}
	}

    juce::SelectedItemSet<DraggableComponent*> draggableItemSet;
};
