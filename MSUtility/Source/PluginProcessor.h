/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DSP/ImageWidener.h"
//==============================================================================
/**
*/
class MSUtilityAudioProcessor  : public juce::AudioProcessor,
                                 public juce::AudioProcessorValueTreeState::Listener
//processor class will inherit from the AudioProcessingValueTreeState::Listener which is a special utility class nested in the AudioProcessingValueTreeState. By inheriting from this, our processor will be able to listen out for changes made to the value tree, and, with a little more code any of the parameters it holds.
{
public:
    //==============================================================================
    MSUtilityAudioProcessor();
    ~MSUtilityAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

   void parameterChanged(const juce::String& parameterID, float newValue) override;
//  provides the declaration of the function we need to include, describing it’s return type (void) and two parameters (more on these later).
    
  float midGain {0.0};
  float sidesGain {0.0};
  float width {1.0f};
    
private:
    juce::AudioProcessorValueTreeState treeState; //declare a new instance of the AudioProcessorValueTreeState called treeState under the private access level modifier
   
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
  
    //  void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    juce::dsp::Gain<float>midGainModule;// get rid of these
    juce::dsp::Gain<float>sidesGainModule;// ""
    
    ImageWidener widenerModule; //create instance of class
    
    //==============================================================================
//    juce::AudioParameterChoice *input; //encoding
//    juce::AudioParameterChoice *output; //decoding
//    juce::AudioParameterFloat *width; // slider for stereo width (range 0, -2)
    // advenced parameters to be added
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessor)
};
