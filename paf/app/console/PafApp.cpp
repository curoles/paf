/**
 * @file
 * @brief     PAF Application.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PafApp.h"

#include "paf/lib/generate/Generator.h"
#include "paf/lib/generate/GeneratorFactory.h"

using namespace paf;

void
paf::Application::
test(const juce::ArgumentList& /*args*/)
{
    printf("testing sound...");
    audioManager_.playTestSound();
    printf("DONE\n");
}

void
paf::Application::
play(const juce::ArgumentList& args)
{
    printf("play...\n");

    jassert(args.containsOption("play"));

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

    printf("Audio file format: %s\n", fmt->getFormatName().toRawUTF8());

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

    printf("Player current gain: %.2f\n", player->getGain());

    audioManager_.addAudioCallback(player.get());

    while (source->getNextReadPosition() < source->getTotalLength()) {
        Thread::sleep(100);
    }

    if (1/*!quite*/) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

    device->stop();
    device->close();
}

void
paf::Application::
generate(const juce::ArgumentList& /*args*/)
{
    printf("generate...\n");

    auto src = paf::GeneratorFactory::make(paf::GeneratorFactory::SIN);
    if (src.get() == nullptr) {
        printf("failed to create generator\n");
        return;
    }

    paf::Generator generator(std::move(src));
}
