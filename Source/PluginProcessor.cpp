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
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     )
#endif
    , apvts(*this, nullptr, "PARAMETERS", createStateLayout())
{
#ifdef DEBUG
    juce::UnitTestRunner runner;
#endif // DEBUG
    apvts.state.addChild(fileTree, 0, nullptr);
    fileListener.reset(new FileListener(this, fileTree));

    for (int i{ 0 }; i < NUM_NODES; i++) {
        generators.add(std::unique_ptr<GrainGenerator>(
            new GrainGenerator(DUMMYSAMPLERATE)));
            apvts.addParameterListener("active" + juce::String{ i }, generators[i]->getActiveParamListener());
            apvts.addParameterListener("numVoices" + juce::String{ i }, generators[i]->getGrainVoiceParamListener());
    }
    apvts.getParameterAsValue("active0").setValue(true);
	apvts.getParameterAsValue("active1").setValue(true);

    
    fileChoiceHandler.reset(new FileChoiceHandler{ fileTree });
    fileChoiceHandler->loadSoundFileToMemory("pretty_rhodes_delay", "C:/Users/Alex/Music/borderlands_defaults/pretty_rhodes_delay.wav");
    fileChoiceHandler->loadSoundFileToMemory("hidden_mechanics_stems_borderlands_stereo", "C:/Users/Alex/Music/borderlands_defaults/hidden_mechanics_stems_borderlands_stereo.wav");
    fileChoiceHandler->loadSoundFileToMemory("sin", "C:/Users/Alex/Music/Samples/440hz sin.wav");

    util::addAudioFileToTree(&fileTree.getChildWithName("sin"), 0, 0, 0, DUMMYSAMPLERATE * 2);
	util::addAudioFileToTree(&fileTree.getChildWithName("pretty_rhodes_delay"), 0, 0, 0, DUMMYSAMPLERATE * 1);
    util::addAudioFileToTree(&fileTree.getChildWithName("pretty_rhodes_delay"), 0, 0, 0, DUMMYSAMPLERATE * 10);

    generators[0]->addActiveSound(allSounds[0]);
    generators[0]->addActiveSound(allSounds[1]);
    generators[0]->addActiveSound(allSounds[2]);
    DBG(apvts.state.toXmlString());
    DBG("HAHHAHA");
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

void ScreenAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ScreenAudioProcessor::getProgramName (int index)
{
    return {};
}

void ScreenAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ScreenAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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
bool ScreenAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void ScreenAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
    /*for (int i{ 0 }; i < buffer.getNumSamples(); i++) 
    {
        for (int channel{ 0 }; channel < 2; channel++) 
        {
            auto writePointer = buffer.getWritePointer(channel);
            auto limitedSample = limiters[channel]->calculateSample(writePointer[i], 0.001f, 0.001f, 0.2f);
            writePointer[i] = limitedSample;
        }
    }*/
    counter++;
}

//==============================================================================
bool ScreenAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ScreenAudioProcessor::createEditor()
{
    return new ScreenAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void ScreenAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ScreenAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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


juce::AudioProcessorValueTreeState::ParameterLayout ScreenAudioProcessor::createStateLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (int i{ 0 }; i < NUM_NODES; i++) {
        //Generator
        layout.add(std::make_unique<juce::AudioParameterBool>("active" + juce::String{ i }, 
            "Generator " + juce::String{ i } + " Active " , false));
        layout.add(std::make_unique<juce::AudioParameterFloat>("size" + juce::String{ i }, 
            "Generator" + juce::String{ i } +" Grain Size", 
            juce::NormalisableRange<float>(5.0f, 5000.0f), 1000.0f));
        layout.add(std::make_unique<juce::AudioParameterInt>("numVoices" + juce::String{ i }, 
            "Generator" + juce::String{ i } + "Grain Voices" , 0, 24, 0));
        layout.add(std::make_unique<juce::AudioParameterFloat>("x" + juce::String{ i }, 
            "Generator" + juce::String{ i } + " X Position",
            juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
        layout.add(std::make_unique<juce::AudioParameterFloat>("y" + juce::String{ i }, 
            "Generator" + juce::String{ i } + " Y Position",
            juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    }
    //setUpSourceValueTree();
    //setUpFileValueTree();
    return layout;
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

/*void ScreenAudioProcessor::setUpSourceValueTree()
{
    juce::ValueTree sTree(Ids::sourceTree);
    sTree.addListener(&sourceListener);
    state.state.addChild(sTree, state.state.getNumChildren(), nullptr);
}

void ScreenAudioProcessor::setUpFileValueTree()
{
    juce::ValueTree fileTree{Ids::fileTree};
    fileTree.addListener(&fileListener);
    state.state.addChild(fileTree, state.state.getNumChildren(), nullptr);
}*/
