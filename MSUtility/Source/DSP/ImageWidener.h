/*
  ==============================================================================

    ImageWidener.h
    Created: 23 Mar 2023 10:24:29am
    Author:  High sierra

  ==============================================================================
*/

#pragma once

#ifndef ImageWidener_h
#define ImageWidener_h

#include <JuceHeader.h>

class ImageWidener
{
public:
    void prepare(juce::dsp::ProcessSpec &spec);
    void processBlock(juce::dsp::AudioBlock<float>& block);
    
    enum class parameterID
    {
        kWidth,
        kBypass

    };
    
    void setParameter(parameterID parameter, float parameterValue);
    
private:
    
    void assertRange(float sourceValue, float min, float max);
    void assertBool(float sourceValue);
    
    juce::SmoothedValue<float> width {1.0f};
    
    bool bypassModule {false};
    float currentSampleRate;
};


#endif // ImageWidener
