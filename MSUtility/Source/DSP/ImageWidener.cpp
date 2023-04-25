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

        
        // get reference of right and left channel into variables
        auto* left = block.getChannelPointer (0);
        auto* right = block.getChannelPointer (1);
        
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            float LowMidRaw = left[sample];
            float LowSidesRaw = right[sample];
            
            if (InChoice == 2)
            {
                LowMidRaw = 0.5 * (left[sample] + right[sample]);
                LowSidesRaw = 0.5 * (left[sample] - right[sample]);
            }
//            else
//            {
//                LowMidRaw = (left[sample]);
//                LowSidesRaw = (right[sample]);
//            }
            
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
            
            const auto OutHighLeft =  newHighMid + newHighSides;
            const auto OutHighRight = newHighMid - newHighSides;
            

            left[sample] =  OutLowLeft + OutHighLeft ;
            right[sample] = OutLowRight + OutHighRight;
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
        
        case parameterID::kInChoice:
        {
            InChoice = parameterValue;
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
