/**
 * @file
 * @brief     PAF Application.
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

    if (!quiet_) {
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

    if (!quiet_) {
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

    if (!quiet_) {
        printf("Player current gain: %.2f\n", player->getGain());
    }

    audioManager_.addAudioCallback(player.get());

    printf("playing %s...\n", inputFile.getFileName().toRawUTF8());

    while (source->getNextReadPosition() < source->getTotalLength()) {
        Thread::sleep(100);
    }

    if (!quiet_) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

    device->stop();
    device->close();
}

void
paf::Application::
generate(const juce::ArgumentList& args)
{
    collectOptionsForGenerate(args);

    auto src = paf::GeneratorFactory::make(paf::GeneratorFactory::SIN);
    if (src.get() == nullptr) {
        printf("failed to create generator\n");
        return;
    }

    paf::Generator generator(std::move(src));

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

    if (!quiet_) {
        printf("Audio device name: %s\n  type: %s\n  sampling rate: %.1f\n  bit depth: %d\n",
            device->getName().toRawUTF8(), device->getTypeName().toRawUTF8(),
            device->getCurrentSampleRate(), device->getCurrentBitDepth());
    }

    generator.source_->prepareToPlay(
        device->getDefaultBufferSize(),
        device->getCurrentSampleRate()
    );

    if (!quiet_) {
        printf("Player current gain: %.2f\n", generator.player_.getGain());
    }

    audioManager_.addAudioCallback(&generator.player_);

    printf("generating...\n");

    device->start(&generator.player_);
    Thread::sleep(option_.durationMs);

    if (!quiet_) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

    device->stop();
    device->close();

    audioManager_.closeAudioDevice();
}

void
paf::Application::
collectOptionsForTest(const juce::ArgumentList& args)
{
    if (args.containsOption("--quiet|-q")) {
        quiet_ = true;
    }

    option_.durationMs = 3000ul;

    if (args.containsOption("--duration|-d")) {
        unsigned long inputVal = (unsigned long)(
            args.getValueForOption("--duration|-d").getLargeIntValue() * 1000);
        option_.durationMs = std::max(inputVal, 3000ul);
    }

}

void
paf::Application::
collectOptionsForPlay(const juce::ArgumentList& args)
{
    if (args.containsOption("--quiet|-q")) {
        quiet_ = true;
    }
}

void
paf::Application::
collectOptionsForGenerate(const juce::ArgumentList& args)
{
    if (args.containsOption("--quiet|-q")) {
        quiet_ = true;
    }

    option_.durationMs = 3000ul;

    if (args.containsOption("--duration|-d")) {
        unsigned long inputVal = (unsigned long)(
            args.getValueForOption("--duration|-d").getLargeIntValue() * 1000);
        option_.durationMs = std::max(inputVal, 3000ul);
    }
}
