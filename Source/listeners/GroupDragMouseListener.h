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
			if (draggableComponent->readyToDrag) 
			{
				if (!draggableItemSet.isSelected(draggableComponent))
				{
					draggableItemSet.deselectAll();
					draggableItemSet.addToSelection(draggableComponent);
				}

				componentBeingDragged = draggableComponent;

				mouseDownWithinTarget = e.getEventRelativeTo(componentBeingDragged).getPosition();

				for (auto component : draggableItemSet)
				{
					if (component != e.eventComponent)
					{
						component->mouseDownWithinTarget = e.getEventRelativeTo(component).getPosition();
					}
				}
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
					if (component != componentBeingDragged)
					{
						handleMouseDrag(component, e, dragDelta + component->mouseDownWithinTarget);
					}
					else
					{
						handleMouseDrag(component, e, mouseDownWithinTarget);
					}
				}
			}
		}
	}

	juce::Point<int> calculateDragDelta(DraggableComponent* component, const juce::MouseEvent e)
	{
		return e.getEventRelativeTo(component).getPosition() - mouseDownWithinTarget;
	}

	void handleMouseDrag(DraggableComponent* component, const juce::MouseEvent e, const juce::Point<int>& dragDelta)
	{
		auto bounds = component->getBounds();
		bounds += e.getEventRelativeTo(component).getPosition() - dragDelta;
		auto x = bounds.getX();
		x = std::max(x, 0);
		x = std::min(x, component->getParentWidth() - component->getWidth());

		auto y = bounds.getY();
		y = std::max(y, 0);
		y = std::min(y, component->getParentHeight() - component->getHeight());

		bounds.setX(x);
		bounds.setY(y);
		component->setValueTreeProperty(Ids::x, (float)component->getBounds().getX() / (component->getParentWidth() - component->getWidth()));
		component->setValueTreeProperty(Ids::y, (float)component->getBounds().getY() / (component->getParentHeight() - component->getHeight()));
		component->setBounds(bounds);
		component->getParentComponent()->repaint();
	}

	juce::SelectedItemSet<DraggableComponent*> draggableItemSet;
	juce::Point<int> mouseDownWithinTarget;
	juce::Point<int> totalDragDelta;
	DraggableComponent* componentBeingDragged{ nullptr };
};
