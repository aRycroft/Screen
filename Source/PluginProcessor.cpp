/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScreenAudioProcessor::ScreenAudioProcessor()
	:
#ifndef JucePlugin_PreferredChannelConfigurations
	AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	vTree.addChild(fileTree, TreeChildren::fileTree, nullptr);
	vTree.addChild(genTree, TreeChildren::genTree, nullptr);
	fileListener.reset(new FileListener(this, fileTree));
	genListener.reset(new GenListener(this, genTree));

	fileChoiceHandler.reset(new FileChoiceHandler{ fileTree });
	fileChoiceHandler->loadSoundFileToMemory("pretty_rhodes_delay", "C:/Users/Alex/Music/borderlands_defaults/pretty_rhodes_delay.wav");
	fileChoiceHandler->loadSoundFileToMemory("hidden_mechanics_stems_borderlands_stereo", "C:/Users/Alex/Music/borderlands_defaults/hidden_mechanics_stems_borderlands_stereo.wav");
	fileChoiceHandler->loadSoundFileToMemory("sin", "C:/Users/Alex/Music/Samples/440hz sin.wav");

	util::addAudioFileToTree(&fileTree.getChildWithName("sin"), 0, 0, 0, DUMMYSAMPLERATE * 2);
	util::addAudioFileToTree(&fileTree.getChildWithName("pretty_rhodes_delay"), 0, 0, 0, DUMMYSAMPLERATE * 1);
	util::addAudioFileToTree(&fileTree.getChildWithName("pretty_rhodes_delay"), 0, 0, 0, DUMMYSAMPLERATE * 10);
}

ScreenAudioProcessor::~ScreenAudioProcessor()
{
}

//==============================================================================
const juce::String ScreenAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool ScreenAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ScreenAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ScreenAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ScreenAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ScreenAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int ScreenAudioProcessor::getCurrentProgram()
{
	return 0;
}

void ScreenAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ScreenAudioProcessor::getProgramName(int index)
{
	return {};
}

void ScreenAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ScreenAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void ScreenAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ScreenAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void ScreenAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	buffer.clear();
	for (auto grainGen : generators)
	{
		if (grainGen->isActive) {
			if (counter % 100 == 0) {
				grainGen->playGrain();
			}
			grainGen->fillNextBuffer(&buffer);
		}
	}
	counter++;
}

//==============================================================================
bool ScreenAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ScreenAudioProcessor::createEditor()
{
	return new ScreenAudioProcessorEditor(*this, vTree);
}

//==============================================================================
void ScreenAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ScreenAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ScreenAudioProcessor();
}

void ScreenAudioProcessor::addAudioBuffer(juce::AudioSampleBuffer newSampleBuffer)
{
	fileBuffers.add(std::make_unique<juce::AudioSampleBuffer>(newSampleBuffer));
}

juce::AudioSampleBuffer* ScreenAudioProcessor::getAudioSampleBuffer(int index)
{
	return fileBuffers[index];
}

void ScreenAudioProcessor::addAudioFile(AudioFile newAudioFile)
{
	allSounds.add(std::make_unique<AudioFile>(newAudioFile));
}

void ScreenAudioProcessor::removeAudioFile(juce::File newFile)
{
}

void ScreenAudioProcessor::createGrainGenerator(juce::ValueTree generatorValueTree) 
{
	generators.add(new GrainGenerator{ DUMMYSAMPLERATE, generatorValueTree });
	generators.getLast()->addActiveSound(allSounds[1]);
	generatorValueTree.setProperty(Ids::numVoices, 1, nullptr);
	generatorValueTree.setProperty(Ids::active, true, nullptr);
}
void ScreenAudioProcessor::removeGrainGenerator(int indexToRemove) 
{
	generators.remove(indexToRemove);
}

