/*
  ==============================================================================

    ConnectionChangeListener.h
    Created: 24 Mar 2021 8:27:55pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "IConnectionHandler.h"
#include "Utils.h"

class ConnectionChangeListener : public juce::ValueTree::Listener 
{
public:
    ConnectionChangeListener(IConnectionHandler* handler, juce::ValueTree conTree, juce::ValueTree genTree)
        :connectionTree(conTree), grainGenTree(genTree)
    {
        connectionTree.addListener(this);
        _handler = handler;
    }

    void valueTreePropertyChanged(juce::ValueTree& vTree, const juce::Identifier& property) override 
    {
        int fromNo = vTree.getProperty(Ids::from);
        int toNo = vTree.getProperty(Ids::to);
        if (property == Ids::weight) {
            float fWeight = vTree.getProperty(Ids::weight);
            setWeight(fromNo, toNo, fWeight);
        }
    }

    void setWeight(int from, int to, double mult)
    {
        DBG(grainGenTree.toXmlString());
        double fromX = grainGenTree.getChild(from).getProperty(Ids::x);
        double fromY = grainGenTree.getChild(from).getProperty(Ids::y);

        double toX = grainGenTree.getChild(to).getProperty(Ids::x);
        double toY = grainGenTree.getChild(to).getProperty(Ids::y);

        float weight = sqrt(pow(toX - fromX, 2) + pow(toY - fromY, 2) * 1.0f);
        
        _handler->connectionWeightChanged(from, to, std::max<float>((1 - weight) * mult, 0.0f));
    }

private:
    juce::ValueTree connectionTree, grainGenTree;
    IConnectionHandler* _handler;
};
