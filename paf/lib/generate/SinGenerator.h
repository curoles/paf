/**
 * @file
 * @brief     Sine wave generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 *
 * @see https://docs.juce.com/master/classToneGeneratorAudioSource.html
 */
#pragma once

#include "paf/lib/generate/Generator.h"

namespace paf {


class SinGenerator : public paf::GeneratorSource
{
    juce::ToneGeneratorAudioSource toneGen_;

public:
    SinGenerator():
        GeneratorSource()
    {
    }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        toneGen_.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void releaseResources() override {
        toneGen_.releaseResources();
    }

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override {
        toneGen_.getNextAudioBlock(bufferToFill);
    }

    void setAmplitude(float newAmplitude) override {
        toneGen_.setAmplitude(newAmplitude);
    }

    void setFrequency(double newFrequencyHz) override {
        toneGen_.setFrequency(newFrequencyHz);
    }

};


} // namespace paf
