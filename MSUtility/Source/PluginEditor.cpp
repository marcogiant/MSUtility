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
  
    // Mode Selection
    InSel.addItem("Stereo", 1.0f);
    InSel.addItem("MS", 2.0f);
    InChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Input", InSel);
    addAndMakeVisible(&InSel);
    
    OutSel.addItem("Stereo", 1.0f);
    OutSel.addItem("MS", 2.0f);
    OutChoice = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(treeState, "Output", OutSel);
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

}

//==============================================================================
void MSUtilityAudioProcessorEditor::paint (juce::Graphics& g)
{
 
    g.fillAll(juce::Colours::greenyellow);
    g.setColour(juce::Colours::black);
    
    //Title text
    g.setFont(30);
    g.drawFittedText("Mid-Side Utility", 160, 20, 210, 10, juce::Justification::centred, 1, 0.0f);
    
    // labels
    g.setFont(25);
    g.drawFittedText("Width", 125, 175, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Low Band", -10, 125, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("High Band", -10, 225, 220, 50, juce::Justification::centred, 1, 0.0f);
     g.drawFittedText("Crossover", 125, 275, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Input", -50, 45, 220, 50, juce::Justification::centred, 1, 0.0f);
    g.drawFittedText("Output", 200, 400, 220, 50, juce::Justification::centred, 1, 0.0f);
}

void MSUtilityAudioProcessorEditor::resized()
{

//    widthSlider.setBounds(100, 50, 220, 50);
    InSel.setBounds(100, 50, 100, 35);
    OutSel.setBounds(360, 410, 100, 35);
    LowWidthSlider.setBounds(150, 100, 250, 100);
    HighWidthSlider.setBounds(150, 200, 250, 100);// x y width height
    CrossfadeSlider.setBounds(150, 300, 250, 100);
}
