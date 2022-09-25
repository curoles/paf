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

    unsigned int numChannels_ = 2;
    float samplingRate_ = 48'000.0;

public:
    AudioFileWriter(unsigned int numChannels=2):numChannels_(numChannels)
    {
        formatManager_.registerBasicFormats();
    }

    enum class Result {OK, NO_ACCESS, FAIL_CREATE, FAIL_OPEN,
        UNKNOWN_FORMAT, FAIL_OPEN_STREAM};

    Result open(
        const String& absolutePath,
        unsigned int numChannels = 2,
        float samplingRate = 48'000.0,
        unsigned int samplingBits = 24
    )
    {
        numChannels_ = numChannels;
        samplingRate_ = samplingRate;

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
            samplingRate_,
            numChannels_,
            (int)samplingBits,
            {},
            0
        ));

        if (writer_ == nullptr) return Result::FAIL_OPEN_STREAM;

        return result;
    }

    void close()
    {
        if (writer_ != nullptr) { writer_->flush(); }
        if (fileOutputStream_ != nullptr) { fileOutputStream_->flush(); }
        writer_.release();
        fileOutputStream_.release();
    }

    void makeThreadedWriter()
    {
        //threadedWriter_.reset(
        //    new AudioFormatWriter::ThreadedWriter(
        //        writer_, backgroundThread,
        //            32768)); fileOutputStream.getBufferSize() * 4
    }

    bool writeFromAudioSampleBuffer(
        const AudioBuffer<float>& audioBuffer,
        unsigned int startSample = 0,
        unsigned int numSamples = 0
    )
    {
        bool result{false};

        if (writer_ != nullptr) {
            result = writer_->writeFromAudioSampleBuffer(
                audioBuffer,
                (int)startSample,
                (int)(numSamples? numSamples : (unsigned int)audioBuffer.getNumSamples())
            );
        }

        return result;
    }

    bool writeFromAudioSource(
        AudioSource& source,
        unsigned int numSamplesToRead,
        unsigned int samplesPerBlock = 2*1024
    )
    {
        bool result{false};

        if (writer_ != nullptr) {
            result = writer_->writeFromAudioSource(
                source,
                (int)numSamplesToRead,
                (int)samplesPerBlock
            );
        }

        return result;
    }

    bool writeFromAudioSourceWithTimeout(
        AudioSource& source,
        float durationMs,
        unsigned int samplesPerBlock = 2*1024
    )
    {
        const unsigned int numSamplesToRead = (durationMs * samplingRate_) / 1000;

        return writeFromAudioSource(
            source,
            numSamplesToRead,
            samplesPerBlock
        );
    }

    //bool writeFromFloatArrays(const float *const *channels, int numChannels, int numSamples)
    //bool writeFromAudioReader (AudioFormatReader &reader, int64 startSample, int64 numSamplesToRead)
};

} // namespace paf
