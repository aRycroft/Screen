/*
  ==============================================================================

    DelayBuffer.cpp
    Created: 27 Mar 2020 1:02:36pm
    Author:  Alex

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "DelayBuffer.h"

DelayBuffer::DelayBuffer() {
    buffer = juce::AudioSampleBuffer();
}
DelayBuffer::DelayBuffer(int bufferSize, int delayOffset)
{
    buffer = juce::AudioSampleBuffer(1, bufferSize);
    buffer.clear();
    writeIndex = delayLength = delayOffset;
    readIndex = 0;
}

void DelayBuffer::setDelayLength(int length)
{
    delayLength = length;
}

float DelayBuffer::getData()
{
    return buffer.getSample(0, readIndex);
}

void DelayBuffer::setData(float data)
{
    buffer.setSample(0, writeIndex, data);
}

void DelayBuffer::nextSample() 
{
    int bufferLength = buffer.getNumSamples();
    readIndex = ((bufferLength + writeIndex) - delayLength) % bufferLength;
    writeIndex = (writeIndex + 1) % bufferLength;
}
