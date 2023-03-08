/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessorEditor::MSUtilityAudioProcessorEditor (MSUtilityAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) //Constructor header changed to accept second argument as per Editor header file changed before
    : AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)//treeState reference variable initialised with treeState reference passed from the Processor class
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

MSUtilityAudioProcessorEditor::~MSUtilityAudioProcessorEditor()
{
    // Width
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "width", widthSlider);
    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setRange(0.0f, 2.0f, 1.0f);
    widthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&widthSlider);
    
    addAndMakeVisible(widthLabel);
   widthLabel.setText("width", juce::dontSendNotification);
   widthLabel.attachToComponent(&widthSlider, false);
    
    //buttons + other parameters -- check practical 3-4
}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
  
    g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::white);
    g.setFont(25);
    g.drawFittedText("Mid-Side Utiliy", 150, 20, 130, 30, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    widthSlider.setBounds(50, 110, 320, 50);
    //feedbackSlider.setBounds(50, 180, 320, 50);
}
