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
        kLowWidth,
        kHighWidth,
        kCrossfade,
        kInChoice,
        kOutChoice,
        kBypass

    };
    
    void setParameter(parameterID parameter, float parameterValue);
    
private:
    
    void assertRange(float sourceValue, float min, float max);
    void assertBool(float sourceValue);
    
    juce::SmoothedValue<float> LowWidth {1.0f};
    juce::SmoothedValue<float> HighWidth {1.0f};
    juce::SmoothedValue<float> crossfadeFreq {10000.f};
    int InChoice {0};
    int OutChoice {0};
    
    bool bypassModule {false};
    float currentSampleRate;
    
    juce::dsp::LinkwitzRileyFilter<float> crossfadeFilterModule;
    juce::dsp::LinkwitzRileyFilter<float> crossfadeFilterModule2;
};


#endif // ImageWidener
