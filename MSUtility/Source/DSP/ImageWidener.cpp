/*
  ==============================================================================

    ImageWidener.cpp
    Created: 23 Mar 2023 10:24:29am
    Author:  High sierra

  ==============================================================================
*/

#include "ImageWidener.h"

void ImageWidener::prepare(juce::dsp::ProcessSpec &spec)
{
    currentSampleRate = spec.sampleRate;
    width.reset(currentSampleRate,  0.02);
    
}

void ImageWidener::processBlock(juce::dsp::AudioBlock<float> &block)
{
    if (bypassModule) return; //if bypass module flags on 'true', return out of processBlock
    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
     //   auto* channelData = buffer.getWritePointer (channel);
        
        // get reference of right and left channel into variables
        auto* left = block.getChannelPointer (0);
        auto* right = block.getChannelPointer (1);
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            const auto mid = 0.5 * (left[sample] + right[sample]);
            const auto sides = 0.5 * (left[sample] - right[sample]);
            
            const auto newMid = (2.0 - width.getNextValue()) * (mid);
            const auto newSides = (width.getNextValue()) * (sides);
            
            const auto OutLeft =  newMid + newSides;
            const auto OutRight = newMid - newSides;
            
            if (width.getNextValue() >= 1.0f)
            {
                left[sample] = OutLeft;
                right[sample] = OutRight;
            }
            else
            {
                const auto volumeScaler = juce::jmap(width.getNextValue(), 1.0f, 0.0f, 0.0f, -8.0f);
                
                left[sample] = OutLeft * juce::Decibels::decibelsToGain(volumeScaler);
                right[sample] = OutRight * juce::Decibels::decibelsToGain(volumeScaler);
            }
            
            
        }
    }
}
void ImageWidener::setParameter(parameterID parameter, float parameterValue)
{
    switch (parameter)
    {
        case parameterID::kWidth:
        {
            width.setTargetValue(parameterValue);
            break;
        }
        case parameterID::kBypass:
        {
            bypassModule = parameterValue;
            break;

        }
    }
    
}

void ImageWidener::assertBool(float sourceValue)
{
    
}

void ImageWidener::assertRange(float sourceValue, float min, float max)
{
 
}
