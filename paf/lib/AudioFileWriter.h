/**
 * @file
 * @brief     Write audio stream to a file.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#pragma once

namespace paf {

class AudioFileWriter
{
    AudioFormatManager formatManager_;

    File fileToWriteTo_;
    std::unique_ptr<FileOutputStream> fileOutputStream_;
    std::unique_ptr<AudioFormatWriter> writer_;
    std::unique_ptr<AudioFormatWriter::ThreadedWriter> threadedWriter_;

public:
    AudioBuffer<float> audioBuffer_;

public:
    AudioFileWriter(int numChannels=2, int numSamples=16*1024):
        audioBuffer_(numChannels, numSamples)
    {
        formatManager_.registerBasicFormats();
    }

    enum class Result {OK, NO_ACCESS, FAIL_CREATE, FAIL_OPEN,
        UNKNOWN_FORMAT, FAIL_OPEN_STREAM};

    Result open(const String& absolutePath)
    {
        Result result{Result::OK};

        fileToWriteTo_ = absolutePath;

        if (!fileToWriteTo_.hasWriteAccess()) return Result::NO_ACCESS;

        if (fileToWriteTo_.create().failed()) return Result::FAIL_CREATE;

        AudioFormat* audioFormat =
            formatManager_.findFormatForFileExtension(fileToWriteTo_.getFileExtension());

        if (audioFormat == nullptr) return Result::UNKNOWN_FORMAT;

        fileOutputStream_ = std::make_unique<FileOutputStream>(
            fileToWriteTo_ /*, size_t bufferSizeToUse=16384*/);

        if (fileOutputStream_->failedToOpen()) return Result::FAIL_OPEN;

        writer_.reset(audioFormat->createWriterFor(
            fileOutputStream_.get(),
            48000.0,
            (unsigned int)audioBuffer_.getNumChannels(),
            24,
            {},
            0
        ));

        if (writer_ == nullptr) return Result::FAIL_OPEN_STREAM;

        return result;
    }

    void makeThreadedWriter()
    {
        //threadedWriter_.reset(
        //    new AudioFormatWriter::ThreadedWriter(
        //        writer_, backgroundThread,
        //            32768)); fileOutputStream.getBufferSize() * 4
    }

    bool writeBufferToFile()
    {
        bool result{false};

        if (writer_ != nullptr) {
            result = writer_->writeFromAudioSampleBuffer(
                audioBuffer_, /*start=*/0, audioBuffer_.getNumSamples());
        }

        return result;
    }

    //bool writeFromFloatArrays(const float *const *channels, int numChannels, int numSamples)
    //bool writeFromAudioSampleBuffer (const AudioBuffer< float > &source, int startSample, int numSamples)
    //bool writeFromAudioSource (AudioSource &source, int numSamplesToRead, int samplesPerBlock=2048)
    //bool writeFromAudioReader (AudioFormatReader &reader, int64 startSample, int64 numSamplesToRead)
};

} // namespace paf
