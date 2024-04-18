/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ToneShaper2AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ToneShaper2AudioProcessorEditor (ToneShaper2AudioProcessor&);
    ~ToneShaper2AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ToneShaper2AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneShaper2AudioProcessorEditor)
};
