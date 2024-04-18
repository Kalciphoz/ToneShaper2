/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
using namespace juce::dsp;
using namespace juce::dsp::IIR;

//==============================================================================
ToneShaper2AudioProcessor::ToneShaper2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

ToneShaper2AudioProcessor::~ToneShaper2AudioProcessor()
{
}

//==============================================================================
const juce::String ToneShaper2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ToneShaper2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ToneShaper2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ToneShaper2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ToneShaper2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ToneShaper2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ToneShaper2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void ToneShaper2AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ToneShaper2AudioProcessor::getProgramName (int index)
{
    return {};
}

void ToneShaper2AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ToneShaper2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ProcessSpec spec;

	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getTotalNumInputChannels();
	spec.sampleRate = sampleRate;

	lowBell.prepare(spec);
	highBell.prepare(spec);

	setSettings();
}

void ToneShaper2AudioProcessor::setSettings()
{
	ChainSettings settings = getSettings(apvts);

	float lowGain = Decibels::decibelsToGain(4.5f - 0.9f * settings.midsBalance);
	float highGain = Decibels::decibelsToGain(1.2f * settings.midsBalance - 6.f);
	lowBell.coefficients = Coefficients<float>().makePeakFilter(getSampleRate(), 400.f, 1.f, lowGain);
	highBell.coefficients = Coefficients<float>().makePeakFilter(getSampleRate(), 2500.f, 1.f, highGain);
}

void ToneShaper2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ToneShaper2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void ToneShaper2AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	setSettings();

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	AudioBlock<float> block(buffer);
	//ProcessContextReplacing<float> context(block);

	auto leftBlock = block.getSingleChannelBlock(0);
	ProcessContextReplacing<float> leftContext(leftBlock);

	lowBell.process(leftContext);
	highBell.process(leftContext);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool ToneShaper2AudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ToneShaper2AudioProcessor::createEditor()
{
    return new ToneShaper2AudioProcessorEditor (*this);
}

//==============================================================================
void ToneShaper2AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ToneShaper2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

ChainSettings ToneShaper2AudioProcessor::getSettings(AudioProcessorValueTreeState& apvts)
{
	ChainSettings settings;

	settings.midsBalance = apvts.getRawParameterValue("midsBalance")->load();

	return settings;
}

AudioProcessorValueTreeState::ParameterLayout ToneShaper2AudioProcessor::createParameterLayout()
{
	AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<AudioParameterFloat>("midsBalance", "Mids Balance", NormalisableRange<float>(0.f, 10.f, 0.1f), 5.f));

	return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ToneShaper2AudioProcessor();
}
