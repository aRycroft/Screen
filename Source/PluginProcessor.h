/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#define NUM_NODES 4
#define DUMMYSAMPLERATE 48000

#include <JuceHeader.h>
#include <vector>
#include "Grain.h"
#include "GrainGenerator.h"
#include "Utils.h"
#include "AudioFile.h"
#include "FileChoiceHandler.h"

//==============================================================================
/**
*/
class ScreenAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	ScreenAudioProcessor();
	~ScreenAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	juce::AudioProcessorValueTreeState::ParameterLayout createStateLayout();
	void addAudioBuffer(juce::AudioSampleBuffer newBuffer);
	juce::AudioSampleBuffer* getAudioSampleBuffer(int index);
	void addAudioFile(AudioFile newAudioFile);
	void removeAudioFile(juce::File newFile);

	class FileListener : public juce::ValueTree::Listener
	{
	public:
		FileListener(ScreenAudioProcessor* processor, juce::ValueTree tree)
			: vTree(tree)
		{
			proc = processor;
			vTree.addListener(this);
			formatManager.registerBasicFormats();
		};

		void valueTreeChildAdded(juce::ValueTree& parentTree, juce::ValueTree& childWhichHasBeenAdded) override
		{
			if (parentTree == vTree) {
				auto filePath = childWhichHasBeenAdded.getProperty(Ids::relativePath);
				juce::File newAudioFile{ filePath };
				if (newAudioFile.existsAsFile()) {
					std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(newAudioFile));
					juce::AudioBuffer<float> newBuffer;
					if (reader.get() != nullptr) {
						auto duration = (float)reader->lengthInSamples / reader->sampleRate;           
						newBuffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples); 
						reader->read(&newBuffer,                                                      
							0,                                                               
							(int)reader->lengthInSamples,                                   
							0,                                                                
							true,                                                             
							true);                                                            
					}
					proc->addAudioBuffer(newBuffer);
				}
			}
			/*AudioFile Child tree added*/
			else if (parentTree.isAChildOf(vTree)) {
				int bufferIndex = vTree.indexOf(parentTree);
				auto* buffer = proc->getAudioSampleBuffer(bufferIndex);
				if (buffer != nullptr) {
					AudioFile newAudioFile{ buffer, childWhichHasBeenAdded[Ids::lowSample], childWhichHasBeenAdded[Ids::highSample]};
					proc->addAudioFile(newAudioFile);
				}
			}
		};
	private:
		juce::AudioFormatManager formatManager;
		ScreenAudioProcessor* proc;
		juce::ValueTree vTree;
	};

	juce::AudioProcessorValueTreeState apvts;
	juce::OwnedArray<GrainGenerator> generators;
	juce::OwnedArray<AudioFile> allSounds;
	juce::OwnedArray<juce::AudioSampleBuffer> fileBuffers;
	juce::ValueTree fileTree{ Ids::fileTree };
	std::unique_ptr<FileListener> fileListener;
	std::unique_ptr<FileChoiceHandler> fileChoiceHandler;
	unsigned counter = 0;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ScreenAudioProcessor)
};
