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
    LowWidth.reset(currentSampleRate,  0.02);
    crossfadeFreq.reset(currentSampleRate,0.02);
    
    crossfadeFilterModule.prepare(spec);
    crossfadeFilterModule.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    
    crossfadeFilterModule2.prepare(spec);
    crossfadeFilterModule2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    
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
            
            const auto LowMidRaw = 0.5 * (left[sample] + right[sample]);
            const auto LowSidesRaw = 0.5 * (left[sample] - right[sample]);
            
            crossfadeFilterModule.setCutoffFrequency(crossfadeFreq.getNextValue());
            crossfadeFilterModule2.setCutoffFrequency(crossfadeFreq.getNextValue());
            const auto LowMid = crossfadeFilterModule.processSample(channel, LowMidRaw);
            const auto LowSides = crossfadeFilterModule2.processSample(channel, LowSidesRaw);
            
            const auto newLowMid = (2.0 - LowWidth.getNextValue()) * (LowMid);
            const auto newLowSides = (LowWidth.getNextValue()) * (LowSides);
            
            const auto OutLowLeft =  newLowMid + newLowSides;
            const auto OutLowRight = newLowMid - newLowSides;
            
            //
            const auto HighMid = LowMidRaw - LowMid;
            const auto HighSides = LowSidesRaw - LowSides;
            
            const auto newHighMid = (2.0 - HighWidth.getNextValue()) * (HighMid);
            const auto newHighSides = (HighWidth.getNextValue()) * (HighSides);
            
            const auto OutHighLeft =  newHighMid + newLowSides;
            const auto OutHighRight = newHighSides - newLowSides;
            
            if (HighWidth.getNextValue() >= 1.0f) //LowWidth.getNextValue() >= 1.0f ||
            {
                left[sample] =  OutHighLeft; //OutLowLeft +
                right[sample] =  OutHighRight; //OutLowRight +
            }
            else
            {
                const auto volumeScaler = juce::jmap(HighWidth.getNextValue(), 1.0f, 0.0f, 0.0f, -8.0f);
                
                left[sample] = OutLowLeft + OutHighLeft * juce::Decibels::decibelsToGain(volumeScaler);
                right[sample] = OutLowRight + OutHighRight * juce::Decibels::decibelsToGain(volumeScaler);
            }
            
            
        }
    }
}
void ImageWidener::setParameter(parameterID parameter, float parameterValue)
{
    switch (parameter)
    {
        case parameterID::kLowWidth:
        {
            LowWidth.setTargetValue(parameterValue);
            break;
        }
            
        case parameterID::kHighWidth:
        {
            HighWidth.setTargetValue(parameterValue);
            break;
        }
            
        case parameterID::kCrossfade:
        {
            crossfadeFreq.setTargetValue(parameterValue);
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
