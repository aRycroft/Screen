/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScreenAudioProcessorEditor::ScreenAudioProcessorEditor(ScreenAudioProcessor& p, juce::AudioProcessorValueTreeState& state)
	: AudioProcessorEditor(&p), audioProcessor(p), apvts(state)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setResizable(true, true);

	mainPanel.reset(new MainPanel{});
	apvts.addParameterListener("active0", mainPanel.get());
	addAndMakeVisible(*mainPanel);
	updateFromValueTree();
	setSize(400, 300);
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
	//generatorVis->setVisible(true);
	mainPanel->setBounds(getBounds());
	/*for each (auto grainVis in generatorV)
	{
		if (grainVis != nullptr) {
			grainVis->setBounds(0, 0, 500, 500);
			DBG("HAH");
		}

	}*/
	//for each (auto child in getChildren())
	//{
	//	DBG("UAH");
	//}
}

void ScreenAudioProcessorEditor::updateFromValueTree()
{
	for (int i{ 0 }; i < NUM_NODES; i++) 
	{
		if (apvts.getParameterAsValue("active" + juce::String{ i }).getValue()) {
			mainPanel->addGeneratorVis();
		}
	}
}
