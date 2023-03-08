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
    
  
//  width
//
//    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
//    (treeState, "width", widthSlider);
//    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
//    widthSlider.setRange(0.0f, 2.0f, 1.0f);
//    widthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
//    addAndMakeVisible(&widthSlider);
//
//    addAndMakeVisible(widthLabel);
//    widthLabel.setText("width", juce::dontSendNotification);
//    widthLabel.attachToComponent(&widthSlider, false);
//
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
    
    // Mode Selection
    InSel.addItem("Stereo", 1);
    InSel.addItem("Mid-Side", 2);
    InChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Input", InSel);
     addAndMakeVisible(&InSel);
   
    OutSel.addItem("Stereo", 1);
    OutSel.addItem("Mid-Side", 2);
    OutChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Output", OutSel);
    addAndMakeVisible(&OutSel);
}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
  
    g.fillAll(juce::Colours::magenta);
    g.setColour(juce::Colours::white);
    g.setFont(25);
    g.drawFittedText("Mid-Side Utiliy", 150, 20, 130, 30, juce::Justification::centred, 1, 0.0f);
    
    // labels
    g.setFont(25);
    g.drawFittedText("Width", 55, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Input", 165, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Output", 55, 175, 12, 12, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    widthSlider.setBounds(50, 110, 320, 50);
    //feedbackSlider.setBounds(50, 180, 320, 50);
    InSel.setBounds(127.5, 169.5, 75, 25);
    OutSel.setBounds(127.5, 169.5, 75, 25);
}
