/*
  ==============================================================================

    ConnectionListener.h
    Created: 14 Mar 2021 8:54:36pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IConnectionHandler.h"
#include "Utils.h"
class ConnectionListener : public juce::ValueTree::Listener
{
public:
	ConnectionListener(IConnectionHandler* handler,
		juce::ValueTree conTree)
		:  connectionTree(conTree)
	{
		_handler = handler;
		connectionTree.addListener(this);
	}

	void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
	{
		_handler->connectionCreated(childWhichHasBeenAdded[Ids::from], childWhichHasBeenAdded[Ids::to]);
	};

private:
	IConnectionHandler* _handler;
	juce::ValueTree connectionTree;
};
