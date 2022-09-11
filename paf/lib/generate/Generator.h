/**
 * @file
 * @brief     Signal generator.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>

namespace paf {

struct GeneratorSource : public juce::AudioSource
{
    virtual void setAmplitude(float newAmplitude) = 0;

    virtual void setFrequency(double newFrequencyHz) = 0;

    static inline
    float* getBufferWrPtrStart(int channel, const juce::AudioSourceChannelInfo& bufferToFill) {
        return bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
    }
};

class Generator
{
public:
    std::unique_ptr<GeneratorSource> source_;

    juce::AudioSourcePlayer player_;

public:
    Generator(std::unique_ptr<GeneratorSource> source):
        source_(std::move(source))
    {
        player_.setSource(source_.get());
    }

};


} // namespace paf
