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

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MSUtilityAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& treeState;// AudioProcessorValueTreeState class reference variable added named ‘treeState’ which will eventually allow access and use of the tree created in the processor class
    
    juce::Slider widthSlider;// 2 x sliders added (one for image width)
    juce::ComboBox InSel;
    juce::ComboBox OutSel;
    juce::Slider MidSlider;
    juce::Slider SideSlider;
    
    juce::Label widthLabel; //2 x labels added (for labelling the above sliders)
    juce::Label InputLabel;
    juce::Label OutputLabel;
    juce::Label midLabel;
    juce::Label sideLabel;
    
    // 2 x AudioProcessorValueTreeState::SliderAttachment (to maintain a connection between the sliders and associated parameters in the ValueTreeState)
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> widthValue; //
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> inChoice;//
    std::unique_ptr <juce::AudioProcessorValueTreeState::ComboBoxAttachment> outChoice;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> midValue;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> sideValue;
    //
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MSUtilityAudioProcessorEditor)
};
