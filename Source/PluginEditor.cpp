/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScreenAudioProcessorEditor::ScreenAudioProcessorEditor(ScreenAudioProcessor& p, juce::ValueTree state)
	: AudioProcessorEditor(&p), audioProcessor(p), vTree(state)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setResizable(true, true);
	screenGUI.reset(new ScreenGUI{vTree});
	addAndMakeVisible(*screenGUI);
	setResizeLimits(200, 200, INT16_MAX, INT16_MAX);
	setSize(1000, 800);
}

ScreenAudioProcessorEditor::~ScreenAudioProcessorEditor()
{
}

//==============================================================================
void ScreenAudioProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ScreenAudioProcessorEditor::resized()
{
	screenGUI->setBounds(getBounds());
}


