/*
  ==============================================================================

    SelectedOption.h
    Created: 22 Aug 2021 1:08:55pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

enum MenuOption
{
    Sample,
    GrainMenu,
    Buffer
};

struct SelectedOption : public juce::ChangeBroadcaster 
{
public:
    MenuOption getMenu() 
    {
        return selectedMenu;
    }

    void setMenu(MenuOption menu) 
    {
        selectedMenu = menu;
        this->sendChangeMessage();
    }

    unsigned selectedMenuId;
    juce::ValueTree valueTree;
private:
    MenuOption selectedMenu;
};
