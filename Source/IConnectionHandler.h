/*
  ==============================================================================

    IConnectionHandler.h
    Created: 14 Mar 2021 8:54:44pm
    Author:  Alex

  ==============================================================================
*/

#pragma once
class IConnectionHandler
{
public:
    virtual void connectionCreated(int from, int to) = 0;
    virtual void connectionRemoved(int from, int to) = 0;
};