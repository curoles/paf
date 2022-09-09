/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

#include <juce_audio_formats/juce_audio_formats.h>

namespace paf {

class Application
{
    juce::AudioDeviceManager audioManager_;
    juce::AudioFormatManager formatManager_;

public:
    Application()
    {
        formatManager_.registerBasicFormats();
    }

    void test(const juce::ArgumentList& args);
    void play(const juce::ArgumentList& args);
    void generate(const juce::ArgumentList& args);

private:
    void playAudioFile(juce::File& file);
};

} // namespace paf
