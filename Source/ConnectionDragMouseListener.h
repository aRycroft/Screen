/*
  ==============================================================================

    IConnectionDragMouseListener.h
    Created: 14 Mar 2021 8:20:29pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IConnectionDragHandler.h"
#include "GrainGeneratorVis.h"

class ConnectionDragMouseListener : public juce::MouseListener
{
public:
	ConnectionDragMouseListener(IConnectionDragHandler* handler)
	{
		_handler = handler;
	};

	void mouseDown(const juce::MouseEvent& event) override
	{
		if (!event.mods.isLeftButtonDown())
		{
			return;
		}
		GrainGeneratorVis* clickedGrainGen = dynamic_cast <GrainGeneratorVis*> (event.eventComponent);
		if (clickedGrainGen != 0)
		{
			if (clickedGrainGen->connectionDrag)
			{
				_handler->startConnectionDrag(clickedGrainGen);
			}
		}
	}

	void mouseUp(const juce::MouseEvent& event) override
	{
		_handler->endConnectionDrag(event);
	}

	void mouseDrag(const juce::MouseEvent& event) override
	{
		if (!event.mods.isLeftButtonDown())
		{
			return;
		}
		_handler->connectionDrag();
	}

private:
	IConnectionDragHandler* _handler;
};
