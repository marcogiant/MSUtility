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

//widthSlider("width", 0.0, 2.0, 0.01, 1.0),

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
        audioProcessor.width = widthSlider.getValue();
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
    

    midValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "midDb", MidSlider);
    MidSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    MidSlider.setRange(-94.0f, 24.0f, 1.0f); //3rd arg is increment
    MidSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&MidSlider);
    MidSlider.onValueChange = [this]()
    {
        audioProcessor.midGain = MidSlider.getValue();

    };

    sideValue = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
    (treeState, "sideDb", SideSlider);
    SideSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    SideSlider.setRange(-94.f, 24.f, 1.f); //3rd arg is increment
    SideSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxRight, true, 75, 25);
    addAndMakeVisible(&SideSlider);
    SideSlider.onValueChange = [this]()
    {
        audioProcessor.sidesGain = SideSlider.getValue();
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
    g.drawFittedText("Width", 50, 185, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Input", 165, 85, 10, 10, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Output", 55, 175, 12, 12, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    widthSlider.setBounds(50, 180, 220, 50);
    //feedbackSlider.setBounds(50, 180, 320, 50);
    InSel.setBounds(0.5, 0.5, 100, 25);
    OutSel.setBounds(250.5, 0.5, 100, 25);
    MidSlider.setBounds(100, 100, 220, 50);
    SideSlider.setBounds(100, 150, 220, 50);
}
