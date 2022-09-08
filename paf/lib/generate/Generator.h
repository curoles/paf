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

using GeneratorSource = juce::AudioSource;

class Generator
{
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
