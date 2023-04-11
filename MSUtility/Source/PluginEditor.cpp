/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessorEditor::MSUtilityAudioProcessorEditor
(MSUtilityAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)

// reference to a Value Tree can now be passed in when the editor is instantiated.

: AudioProcessorEditor (&p), audioProcessor (p), treeState(vts)//treeState reference variable initialised with treeState reference passed from the Processor class

{
    setSize (500, 500);
  
//  width
//    widthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
//    (treeState, "width", widthSlider);
//    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
//    widthSlider.setRange(0.0f, 2.0f, 0.0f); //3rd arg is increment
//    widthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
//    addAndMakeVisible(&widthSlider);
//    widthSlider.onValueChange = [this]()
//    {
//        audioProcessor.width = (widthSlider.getValue()); //set a target to ramp to
//    };
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
    
    // LowWidthSlider
    
   LowWidthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "LowWidth", LowWidthSlider);
    LowWidthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    LowWidthSlider.setRange(0.0f, 2.0f, 0.01f); //3rd arg is increment
   LowWidthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&LowWidthSlider);
    LowWidthSlider.setValue(1.0);
    LowWidthSlider.onValueChange = [this]()
    {
        audioProcessor.LowWidth = LowWidthSlider.getValue();
    };
    
//HighWidthSlider
   HighWidthValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "HighWidth", HighWidthSlider);
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
 
    g.fillAll(juce::Colours::greenyellow);
    g.setColour(juce::Colours::black);
    
    //Title text
    g.setFont(30);
    g.drawFittedText("Mid-Side Utility", 10, 20, 210, 10, juce::Justification::centred, 1, 0.0f);
    
    // labels
    g.setFont(25);
    g.drawFittedText("Width", 50, 180, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Input", 165, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Output", 55, 175, 12, 12, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    // lay out the positions of any subcomponents in your editor..
    
//    widthSlider.setBounds(100, 50, 220, 50);
    InSel.setBounds(200, 50, 100, 35);
    OutSel.setBounds(200, 400, 100, 35);
    LowWidthSlider.setBounds(150, 100, 250, 100);
    HighWidthSlider.setBounds(150, 200, 250, 100);// x y width height
    CrossfadeSlider.setBounds(150, 300, 250, 100);
}
