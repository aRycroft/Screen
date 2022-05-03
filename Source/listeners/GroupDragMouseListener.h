/*
  ==============================================================================

	GroupDragMouseListener.h
	Created: 24 Mar 2021 7:16:54pm
	Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class GroupDragMouseListener : public juce::MouseListener
{
public:
	void mouseDown(const juce::MouseEvent& e) override
	{
		if (e.mods.isLeftButtonDown())
		{
			auto draggableComponent = dynamic_cast <DraggableComponent*> (e.eventComponent);
			if (draggableComponent != nullptr && draggableComponent->readyToDrag)
			{
				if (!draggableItemSet.isSelected(draggableComponent))
				{
					draggableItemSet.deselectAll();
					draggableItemSet.addToSelection(draggableComponent);
				}

				componentBeingDragged = draggableComponent;
				componentBeingDragged->mouseDownWithinTarget = e.getEventRelativeTo(componentBeingDragged).getPosition();
			}
		}
	}

	void mouseDrag(const juce::MouseEvent& e) override
	{
		if (componentBeingDragged != nullptr && componentBeingDragged->readyToDrag)
		{
			if (e.mods.isLeftButtonDown())
			{
				auto dragDelta = calculateDragDelta(componentBeingDragged, e);
				for (auto component : draggableItemSet)
				{
					handleMouseDrag(component, e, dragDelta);
				}
			}
		}
	}

	juce::Point<int> calculateDragDelta(DraggableComponent* component, const juce::MouseEvent e)
	{
		auto dragDelta = e.getEventRelativeTo(component).getPosition() - component->mouseDownWithinTarget;

		for (auto component : draggableItemSet)
		{
			auto bounds = component->getBounds();
			auto x = bounds.getX();
			if (x + dragDelta.getX() <= 0)
			{
				dragDelta.setX(-x);
			}
			else if (x + dragDelta.getX() >= component->getParentWidth() - component->getWidth())
			{
				dragDelta.setX(component->getParentWidth() - component->getWidth() - x);
			}

			auto y = bounds.getY();
			if (y + dragDelta.getY() <= 0) 
			{
				dragDelta.setY(-y);
			}
			else if (y + dragDelta.getY() >= component->getParentHeight() - component->getHeight()) 
			{
				dragDelta.setY(component->getParentHeight() - component->getHeight() - y);
			}
		}
		return dragDelta;
	}

	void handleMouseDrag(DraggableComponent* component, const juce::MouseEvent e, juce::Point<int>& dragDelta)
	{
		auto bounds = component->getBounds();
		
		bounds += dragDelta;

		component->setValueTreeProperty(Ids::x, (float) bounds.getX() / (component->getParentWidth() - component->getWidth()));
		component->setValueTreeProperty(Ids::y, (float) bounds.getY() / (component->getParentHeight() - component->getHeight()));
		
		component->getParentComponent()->resized();
		component->getParentComponent()->repaint();
	}

	juce::SelectedItemSet<DraggableComponent*> draggableItemSet;
	juce::Point<int> mouseDownWithinTarget;
	juce::Point<int> totalDragDelta;
	DraggableComponent* componentBeingDragged{ nullptr };
};
