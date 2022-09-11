/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

#include <juce_audio_formats/juce_audio_formats.h>

namespace paf {

struct AppOptions
{
    /// Quite mode, do not show diagnostic messages.
    bool quiet{false};

    unsigned long durationMs{0};
    unsigned short nrChannels{2};
    unsigned short signalType{0};
    float amplitude{1.0};
    float freq{1000.0};
};

class Application
{
    juce::AudioDeviceManager audioManager_;
    juce::AudioFormatManager formatManager_;

    AppOptions option_;

public:
    Application()
    {
        formatManager_.registerBasicFormats();
    }

    void test(const juce::ArgumentList& args);
    void list(const juce::ArgumentList& args);
    void play(const juce::ArgumentList& args);
    void generate(const juce::ArgumentList& args);

    void setQuietMode(bool newMode=true) {
        option_.quiet = newMode;
    }

private:
    void playAudioFile(juce::File& file);

    void collectOptionsForTest(const juce::ArgumentList& args);
    void collectOptionsForPlay(const juce::ArgumentList& args);
    void collectOptionsForGenerate(const juce::ArgumentList& args);
};

} // namespace paf
