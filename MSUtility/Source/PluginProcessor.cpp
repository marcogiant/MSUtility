/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessor::MSUtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       
                        treeState(*this, nullptr, juce::Identifier("PARAMETERS"),
                                  //1 (*this) provides the tree state with a reference to the processor class.
                                  //2 ‘nullptr’ is for the UndoManager, indicating it will not be used in this implementation
                                  //3 unique string identifier used to initialise the internal ValueTree
{
   
    std::make_unique<juce::AudioParameterChoice>("InChoice", "Input", juce::StringArray("Stereo", "MS"), 0),
    std::make_unique<juce::AudioParameterFloat>("LowWidth", "LowWidth", 0.f, 2.f, 0.f),
    std::make_unique<juce::AudioParameterFloat>("HighWidth", "HighWidth", 0.f, 2.f, 0.f),
    std::make_unique<juce::AudioParameterInt>("Hz", "Crossfade", 20, 20000, 1000),
    std::make_unique<juce::AudioParameterChoice>("OutChoice", "Output", juce::StringArray("Stereo", "MS"), 0)
    
    //add std::make_unique<juce::AudioParameterFloat> for Input/Output level
    //perhaps mid or side level individually?
    //it would be good to pan mid or side individually to L/R channels
})

#endif

{
    const juce::StringArray params = {"InChoice", "LowWidth", "HighWidth", "Hz" "OutChoice"};
    for (int i = 0; i <= 4; ++i)
    {
        treeState.addParameterListener(params[i], this);  // adds a listener to each parameter in the array.
    }
        
  
    treeState.addParameterListener("InChoice", this);
    treeState.addParameterListener("OutChoice", this);
    treeState.addParameterListener("LowWidth", this);
    treeState.addParameterListener("HighWidth", this);
    treeState.addParameterListener("Hz", this);
    
}

MSUtilityAudioProcessor::~MSUtilityAudioProcessor()
{
    
}

juce::AudioProcessorValueTreeState::ParameterLayout MSUtilityAudioProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    //update number of reservation for each new added parameter
    params.reserve(4); ///
    return { params.begin(), params.end()};
}

//==============================================================================
const juce::String MSUtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MSUtilityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MSUtilityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MSUtilityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MSUtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MSUtilityAudioProcessor::getNumPrograms()
{
    return 1;
}

int MSUtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MSUtilityAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MSUtilityAudioProcessor::getProgramName (int index)
{
    return {};
}

void MSUtilityAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MSUtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   
    juce::dsp::ProcessSpec spec;
    
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    widenerModule.prepare(spec);
}

void MSUtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MSUtilityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MSUtilityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    if(totalNumInputChannels == 2){
        
    juce::dsp::AudioBlock<float>audioBlock {buffer};
        widenerModule.setParameter(ImageWidener::parameterID::kLowWidth, LowWidth);
        widenerModule.setParameter(ImageWidener::parameterID::kHighWidth, HighWidth);
        widenerModule.setParameter(ImageWidener::parameterID::kCrossfade, crossfade);
        widenerModule.setParameter(ImageWidener::parameterID::kInChoice, InChoice);
        widenerModule.setParameter(ImageWidener::parameterID::kOutChoice, OutChoice);
        widenerModule.processBlock(audioBlock);
        
    
    }
}

//==============================================================================
bool MSUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MSUtilityAudioProcessor::createEditor()
{
    return new MSUtilityAudioProcessorEditor (*this, treeState);
}

//==============================================================================
void MSUtilityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void MSUtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MSUtilityAudioProcessor();
}

void MSUtilityAudioProcessor::parameterChanged(const juce::String& parameterID, float
                                                  newValue)
{
   
}

