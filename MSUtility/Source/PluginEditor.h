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
class MSUtilityAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MSUtilityAudioProcessorEditor (MSUtilityAudioProcessor&, juce::AudioProcessorValueTreeState&);// Constructor changed to accept a second argument which is a reference type of juce::AudioProcessorValueTreeState&
    ~MSUtilityAudioProcessorEditor() override;


    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
//    juce::Slider cutoffDial;
//    juce::Slider resonanceDial;
//    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> cutoffValue;
//    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> resonanceValue;
//    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> driveValue;
//    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeChoice;
//
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MSUtilityAudioProcessor& audioProcessor;

    juce::AudioProcessorValueTreeState& treeState;// AudioProcessorValueTreeState class reference variable added named ‘treeState’ which will eventually allow access and use of the tree created in the processor class
    
    juce::Slider widthSlider;// 2 x sliders added (one for stereo width)
    juce::ComboBox InSel;
    juce::ComboBox OutSel;
    //  juce::Slider p2Slider;
    
    juce::Label widthLabel; //2 x labels added (for labelling the above sliders)
    juce::Label InputLabel;
    juce::Label OutputLabel;
    //juce::Label p2Label;
    
    // 2 x AudioProcessorValueTreeState::SliderAttachment (to maintain a connection between the sliders and associated parameters in the ValueTreeState)
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> widthValue; //
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> InChoice;//
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> OutChoice;
     // std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> p2Value;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessorEditor)
};
