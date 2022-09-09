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
play(const juce::ArgumentList& /*args*/)
{
    printf("play...\n");
    //File input = File::getSpecialLocation (File::userHomeDirectory).getChildFile ("bongo.wav");
    //File input = File::getCurrentWorkingDirectory().getChildFile ("bongo.wav");
    //AudioFormat* fmt = formatManager_.
    //findFormatForFileExtension (const String &fileExtension) const
    //const String& fmt.getFormatName () const

    playAudioFile();
}

void
paf::Application::
playAudioFile()
{
    static const int nrInputChannels = 0;
    int nrOutputChannels = 2; //FIXME use option
    audioManager_.initialiseWithDefaultDevices(nrInputChannels, nrOutputChannels);

    juce::AudioIODevice* device = audioManager_.getCurrentAudioDevice();
    if (!device) {
        printf("Error: can't find output audio device.\n");
        goto exitGate;
    }

#if 0
    File input ("/Users/whoever/test.wav");
    ScopedPointer<AudioFormatReaderSource> source =
        new AudioFormatReaderSource (fmgr.createReaderFor(input), true);
    if (!source) {
        printf("Error: can't read audio file.\n");
        goto exitGate;
    }

    source->prepareToPlay(
        device->getDefaultBufferSize(),
        device->getCurrentSampleRate()
    );

    ScopedPointer<AudioSourcePlayer> player = new AudioSourcePlayer ();
    player->setSource (source);

    audioManager_.addAudioCallback(player);

    while (source->getNextReadPosition() < source->getTotalLength()) {
        Thread::sleep(100);
    }
#endif

    if (1/*!quite*/) {
        double cpuUsage = audioManager_.getCpuUsage() * 100;
        printf("CPU usage: %.1f %%\n", cpuUsage);
    }

exitGate:
    audioManager_.closeAudioDevice();
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
