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
	screenGUI.reset(new ScreenGUI{vTree});
	addAndMakeVisible(*screenGUI);
	setResizable(true, true);
	setResizeLimits(200, 200, INT16_MAX, INT16_MAX);
	setSize(1000, 800);
	syncState();
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

void ScreenAudioProcessorEditor::syncState()
{
	screenGUI->syncState();
}


