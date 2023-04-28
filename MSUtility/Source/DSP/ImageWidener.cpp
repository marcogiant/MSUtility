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
    
    crossfadeFilterModule.prepare(spec); //create two instances of dsp module LinkwitzRiley
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
            
    if (InChoice == 0) // stereo input
    {
                
            
        {
            // store channel data sample by sample in new variable
            float LowMidRaw = left[sample];
            float LowSidesRaw = right[sample];
        
    //first stage of MS processing pre-filtering
            
                LowMidRaw = 0.5 * (left[sample] + right[sample]);
                LowSidesRaw = 0.5 * (left[sample] - right[sample]);
        
   // combination of two instances create higher order filter
            crossfadeFilterModule.setCutoffFrequency(crossfadeFreq.getNextValue());
            crossfadeFilterModule2.setCutoffFrequency(crossfadeFreq.getNextValue());
            const auto LowMid = crossfadeFilterModule.processSample(channel, LowMidRaw);
            const auto LowSides = crossfadeFilterModule2.processSample(channel, LowSidesRaw); //low pass filtered signals stored in new variables
            
            // mid side processing - low band
            const auto newLowMid = (2.0 - LowWidth.getNextValue()) * (LowMid);
            const auto newLowSides = (LowWidth.getNextValue()) * (LowSides);
            
            //stereo output of MS processed low band
            const auto OutLowLeft =  newLowMid + newLowSides;
            const auto OutLowRight = newLowMid - newLowSides;
            
            
            //subtract lowpassed signal to original to get high pass filtered signal
            const auto HighMid = LowMidRaw - LowMid;
            const auto HighSides = LowSidesRaw - LowSides;
            
            //mid side processing - high band
            const auto newHighMid = (2.0 - HighWidth.getNextValue()) * (HighMid);
            const auto newHighSides = (HighWidth.getNextValue()) * (HighSides);
            
            //stereo output of MS processed high band
            const auto OutHighLeft =  newHighMid + newHighSides;
            const auto OutHighRight = newHighMid - newHighSides;
            
         
            //Decoding
            if (OutChoice == 0) // 0 == stereo
            {
// stereo output
            left[sample] =  OutLowLeft + OutHighLeft ;
            right[sample] = OutLowRight + OutHighRight;
            }
            else
            { // 1 == Mid-Side
// Mid-Side output
            left[sample] = newLowMid + newHighMid; // Mid output
            right[sample] = newLowSides + newHighSides; //Sides output
            }
        }
    }
        else // MS input
        {
        float  Mid = left[sample];
        float  Sides = right[sample];
            
             left[sample] = Mid + Sides;
             right[sample] = Mid - Sides;
          
        float LowMidRaw = left[sample];
        float LowSidesRaw = right[sample];
            
            //first stage of MS processing pre-filtering
            
            LowMidRaw = 0.5 * (left[sample] + right[sample]);
            LowSidesRaw = 0.5 * (left[sample] - right[sample]);
            
            // combination of two instances create higher order filter
            crossfadeFilterModule.setCutoffFrequency(crossfadeFreq.getNextValue());
            crossfadeFilterModule2.setCutoffFrequency(crossfadeFreq.getNextValue());
            const auto LowMid = crossfadeFilterModule.processSample(channel, LowMidRaw);
            const auto LowSides = crossfadeFilterModule2.processSample(channel, LowSidesRaw); //low pass filtered signals stored in new variables
            
            // mid side processing - low band
            const auto newLowMid = (2.0 - LowWidth.getNextValue()) * (LowMid);
            const auto newLowSides = (LowWidth.getNextValue()) * (LowSides);
            
            //stereo output of MS processed low band
            const auto OutLowLeft =  newLowMid + newLowSides;
            const auto OutLowRight = newLowMid - newLowSides;
            
            
            //subtract lowpassed signal to original to get high pass filtered signal
            const auto HighMid = LowMidRaw - LowMid;
            const auto HighSides = LowSidesRaw - LowSides;
            
            //mid side processing - high band
            const auto newHighMid = (2.0 - HighWidth.getNextValue()) * (HighMid);
            const auto newHighSides = (HighWidth.getNextValue()) * (HighSides);
            
            //stereo output of MS processed high band
            const auto OutHighLeft =  newHighMid + newHighSides;
            const auto OutHighRight = newHighMid - newHighSides;
            
            
            //Decoding
            if (OutChoice == 0) // 0 == stereo
            {
                // stereo output
                left[sample] =  OutLowLeft + OutHighLeft ;
                right[sample] = OutLowRight + OutHighRight;
            }
            else
            { // 1 == Mid-Side
                // Mid-Side output
                left[sample] = newLowMid + newHighMid; // Mid output
                right[sample] = newLowSides + newHighSides; //Sides output
            }

        }
//
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
            
        case parameterID::kOutChoice:
        {
            OutChoice = parameterValue;
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
