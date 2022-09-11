/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PafApp.h"

#include <juce_audio_utils/juce_audio_utils.h>

using namespace paf;

void
paf::Application::
test(const juce::ArgumentList& args)
{
    collectOptionsForTest(args);

    audioManager_.initialiseWithDefaultDevices(0, 2);
    juce::AudioIODevice* device = audioManager_.getCurrentAudioDevice();
    if (!device) {
        printf("Error: can't find output audio device.\n");
        return;
    }

    SoundPlayer player;
    audioManager_.addAudioCallback(&player);

    printf("testing sound...");
    audioManager_.playTestSound();
    Thread::sleep(option_.durationMs);
    printf("DONE\n");

    device->stop();
    device->close();
    audioManager_.closeAudioDevice();
}

void
paf::Application::
list(const juce::ArgumentList& /*args*/)
{
    const OwnedArray<AudioIODeviceType>& devtypes = audioManager_.getAvailableDeviceTypes();
    for (AudioIODeviceType *const * devtype = devtypes.begin(); devtype != devtypes.end(); ++devtype) {
        const String& devtypeName = (*devtype)->getTypeName();
        printf("- %s\n", devtypeName.toRawUTF8());
        (*devtype)->scanForDevices();
        StringArray names = (*devtype)->getDeviceNames(/*wantInputNames=*/false);
        for (const String* name = names.begin(); name != names.end(); ++name) {
            printf("  + %s\n", name->toRawUTF8());
        }
    }
}

void
paf::Application::
play(const juce::ArgumentList& args)
{
    collectOptionsForPlay(args);

    juce::File file;
    String filename;
    if (args.containsOption("--file|-f")) {
        filename = args.getValueForOption("--file|-f");
        file = args.getExistingFileForOption("--file|-f");
    }
    else if (args.size() > 1) {
        filename = args[1].text;
        file = File(filename);
    }

    if (!file.existsAsFile()) {
        printf("Error: input file '%s' does not exist\n", filename.toRawUTF8());
        return;
    }

    //File input = File::getSpecialLocation (File::userHomeDirectory).getChildFile ("xxx.wav");
    //File input = File::getCurrentWorkingDirectory().getChildFile ("xxx.wav");

    AudioFormat* fmt = formatManager_.findFormatForFileExtension(file.getFileExtension());
    if (!fmt) {
        printf("Error: can't detect audio file format\n");
        return;
    }

    if (!option_.quiet) {
        printf("Audio file format: %s\n", fmt->getFormatName().toRawUTF8());
    }

    playAudioFile(file);

    audioManager_.closeAudioDevice();
}

void
paf::Application::
playAudioFile(juce::File& inputFile)
{
    static const int nrInputChannels = 0;
    int nrOutputChannels = 2; //FIXME use option

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

    std::unique_ptr<AudioFormatReaderSource> source(
        new AudioFormatReaderSource(formatManager_.createReaderFor(inputFile), true));
    if (source.get() == nullptr) {
        printf("Error: can't read audio file '%s'.\n",
            inputFile.getFileName().toRawUTF8());
        return;
    }

    source->prepareToPlay(
        device->getDefaultBufferSize(),
        device->getCurrentSampleRate()
    );

    std::unique_ptr<AudioSourcePlayer> player(new AudioSourcePlayer());
    player->setSource(source.get());

    if (!option_.quiet) {
        printf("Player current gain: %.2f\n", player->getGain());
    }

    audioManager_.addAudioCallback(player.get());

    printf("playing %s...\n", inputFile.getFileName().toRawUTF8());

    while (source->getNextReadPosition() < source->getTotalLength()) {
        Thread::sleep(100);
    }

    if (!option_.quiet) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

    device->stop();
    device->close();
}

