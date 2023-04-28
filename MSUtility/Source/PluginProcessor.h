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
    //processor class inherits from the AudioProcessingValueTreeState::Listener, a special utility class nested in the AudioProcessingValueTreeState. This way, the processor will be able to listen out for changes made to the value tree
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
// override this function and provide implementation for it
    //provides the declaration of the function we need to include, describing it’s return type (void) and two parameters.
  float InChoice {0.0};
    float OutChoice {0.0};
  float LowWidth {1.0};
  float HighWidth {1.0};
  float width {1.0f};
  float crossfade {1000.0f};
    
private:
    juce::AudioProcessorValueTreeState treeState; //declare a new instance of the AudioProcessorValueTreeState called treeState under the private access level modifier
   
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
  
    ImageWidener widenerModule; //create instance of class
    
    //==============================================================================
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessor)
};
