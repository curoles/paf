/**
 * @file
 * @brief     PAF Application signal generation.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PafApp.h"

#include <juce_audio_utils/juce_audio_utils.h>

#include "paf/lib/generate/Generator.h"
#include "paf/lib/generate/GeneratorFactory.h"

using namespace paf;

void
paf::Application::
generate(const juce::ArgumentList& args)
{
    collectOptionsForGenerate(args);

    option_.signalType = paf::GeneratorFactory::SIN;
    String signalName("sin");
    if (args.size() > 1 and !args[1].text.startsWith("-")) {
        signalName = args[1].text;
        if (signalName == "sin") { option_.signalType = paf::GeneratorFactory::SIN; }
        else if (signalName == "white-noise") { option_.signalType = paf::GeneratorFactory::WHITE_NOISE; }
        else {
            printf("Error: unknown signal '%s', "
                "use sin|white-noise\n", signalName.toRawUTF8());
            return;
        }
    }

    auto src = paf::GeneratorFactory::make((paf::GeneratorFactory::Type)option_.signalType);
    if (src.get() == nullptr) {
        printf("Error: failed to create generator\n");
        return;
    }

    paf::Generator generator(std::move(src));

    static const int nrInputChannels = 0;
    int nrOutputChannels = option_.nrChannels;

    String report = audioManager_.initialiseWithDefaultDevices(
        nrInputChannels, nrOutputChannels);

    if (!report.isEmpty()) {
        printf("Initialization report: '%s'\n", report.toRawUTF8());
    }

    juce::AudioIODevice* device = audioManager_.getCurrentAudioDevice();
    if (!device) {
        printf("Error: can't find output audio device.\n");
        return;
    }

    if (!option_.quiet) {
        printf("Audio device name: %s\n  type: %s\n  sampling rate: %.1f\n  bit depth: %d\n",
            device->getName().toRawUTF8(), device->getTypeName().toRawUTF8(),
            device->getCurrentSampleRate(), device->getCurrentBitDepth());
    }

    generator.source_->prepareToPlay(
        device->getDefaultBufferSize(),
        device->getCurrentSampleRate()
    );

    if (!option_.quiet) {
        printf("Player current gain: %.2f\n", generator.player_.getGain());
    }

    audioManager_.addAudioCallback(&generator.player_);

    printf("generating %s...\n", signalName.toRawUTF8());

    device->start(&generator.player_);
    Thread::sleep(option_.durationMs);

    if (!option_.quiet) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

    device->stop();
    device->close();

    audioManager_.closeAudioDevice();
}

