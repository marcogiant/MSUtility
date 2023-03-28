/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessorEditor::MSUtilityAudioProcessorEditor
(MSUtilityAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) //Constructor header changed to accept second argument as per Editor header file changed before

: AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)//,//treeState reference variable initialised with treeState reference passed from the Processor class



{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
  
//  width
//
    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "width", widthSlider);
    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setRange(0.0f, 2.0f, 0.0f); //3rd arg is increment
    widthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&widthSlider);
    widthSlider.onValueChange = [this]()
    {
        audioProcessor.width = (widthSlider.getValue()); //set a target to ramp to
    };
//
    // Mode Selection
    InSel.addItem("Stereo", 1);
    InSel.addItem("Mid-Side", 2);
    inChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Input", InSel);
    addAndMakeVisible(&InSel);
    
    OutSel.addItem("Stereo", 1);
    OutSel.addItem("Mid-Side", 2);
    outChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Output", OutSel);
    addAndMakeVisible(&OutSel);
    
//    LowWidthSlider("dB", 0.0, 2.0, 0.01, 1.0),
    
   LowWidthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "dB", LowWidthSlider);
    LowWidthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    LowWidthSlider.setRange(0.0f, 2.0f, 0.01f); //3rd arg is increment
   LowWidthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&LowWidthSlider);
    LowWidthSlider.setValue(1.0);
    LowWidthSlider.onValueChange = [this]()
    {
        audioProcessor.LowWidth = LowWidthSlider.getValue(); //change midGain to lowWidthGain

    };
//HighWidthSlider("dB", 0.0, 2.0, 0.01, 1.0),
   HighWidthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "dB", HighWidthSlider);
    HighWidthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    HighWidthSlider.setRange(0.0f, 2.0f, 0.01f); //3rd arg is increment
    HighWidthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&HighWidthSlider);
    HighWidthSlider.setValue(1.0);
    HighWidthSlider.onValueChange = [this]()
    {
        audioProcessor.HighWidth = HighWidthSlider.getValue();//same as above
    };
    
    //CrossfadeSlider(),
    CrossfadeValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "Hz", CrossfadeSlider);
    CrossfadeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    CrossfadeSlider.setRange(20, 20000, 1); //3rd arg is increment
    CrossfadeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&CrossfadeSlider);
    CrossfadeSlider.setSkewFactorFromMidPoint(1000.0);
    CrossfadeSlider.setValue(1000.0);
    CrossfadeSlider.onValueChange = [this]()
    {
        audioProcessor.crossfade = CrossfadeSlider.getValue();//same as above/
    };
}

MSUtilityAudioProcessorEditor::~MSUtilityAudioProcessorEditor()
{
//    // Width
//    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(treeState, "width", widthSlider);
//    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
//    widthSlider.setRange(0.0f, 2.0f, 1.0f);
//    widthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
//    addAndMakeVisible(&widthSlider);
    
//    addAndMakeVisible(widthLabel);
//   widthLabel.setText("width", juce::dontSendNotification);
//   widthLabel.attachToComponent(&widthSlider, false);
//
    //buttons + other parameters -- check practical 3-4
    
//    // Mode Selection
//    InSel.addItem("Stereo", 1);
//    InSel.addItem("Mid-Side", 2);
//    inChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Input", InSel);
//     addAndMakeVisible(&InSel);
//
//    OutSel.addItem("Stereo", 1);
//    OutSel.addItem("Mid-Side", 2);
//    outChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Output", OutSel);
//    addAndMakeVisible(&OutSel);
}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
  
    g.fillAll(juce::Colours::greenyellow);
    g.setColour(juce::Colours::transparentBlack);
    
    //Title text
    g.setFont(30);
    g.drawFittedText("Mid-Side Utiliy", 10, 20, 210, 10, juce::Justification::centred, 1, 0.0f);
    
    // labels
    g.setFont(25);
    g.drawFittedText("Width", 50, 180, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Input", 165, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Output", 55, 175, 12, 12, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    widthSlider.setBounds(100, 50, 220, 50);
    //feedbackSlider.setBounds(50, 180, 320, 50);
    InSel.setBounds(0.5, 0.5, 100, 25);
    OutSel.setBounds(250.5, 0.5, 100, 25);
    LowWidthSlider.setBounds(100, 100, 220, 50);
    HighWidthSlider.setBounds(100, 150, 220, 50);// x y width height
    CrossfadeSlider.setBounds(100, 200, 220, 50);
    
}
