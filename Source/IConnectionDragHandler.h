/*
  ==============================================================================

    IConnectionDragMouseListener.h
    Created: 14 Mar 2021 8:20:07pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include "GrainGeneratorVis.h"
class IConnectionDragHandler 
{
public:
    virtual void startConnectionDrag(GrainGeneratorVis* grainGenVis) = 0;
    virtual void connectionDrag() = 0;
    virtual void endConnectionDrag(const juce::MouseEvent& event) = 0;
};
