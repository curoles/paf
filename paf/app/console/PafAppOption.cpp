/**
 * @file
 * @brief     PAF Application options.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "PafApp.h"

using namespace paf;

enum AppOption {QUIET, CHANNELS, DURATION, AMPLITUDE, FREQ,
    OUT_FILE
};

template<AppOption N>
struct AppOptionStr;

#define DEF_OP(num, str) template<> \
struct AppOptionStr<AppOption::num> { static constexpr const char* value = str; };

DEF_OP(QUIET,     "--quiet|-q")
DEF_OP(CHANNELS,  "--channels|-c")
DEF_OP(DURATION,  "--duration|-d")
DEF_OP(AMPLITUDE, "--amplitude|-a")
DEF_OP(FREQ,      "--freq|-f")
DEF_OP(OUT_FILE,  "--output-file|-o")
#undef DEF_OP


static void parseOptionQuiet(juce::ArgumentList& args, AppOptions& option)
{
    constexpr const char* name = AppOptionStr<AppOption::QUIET>::value;

    if (args.containsOption(name)) {
        option.quiet = true;
        args.removeOptionIfFound(name);
    }
}

static void parseOptionChannels(juce::ArgumentList& args, AppOptions& option)
{
    constexpr const char* name = AppOptionStr<AppOption::CHANNELS>::value;

    if (args.containsOption(name)) {
        unsigned short inputVal = (unsigned short)(
            args.getValueForOption(name).getIntValue());

        option.nrChannels = std::min(std::max(inputVal, (unsigned short)32), (unsigned short)1);

        args.removeValueForOption(name);
        args.removeOptionIfFound(name);
    }
}

static void parseOptionDuration(juce::ArgumentList& args, AppOptions& option,
    unsigned long defaultVal=3000ul, unsigned long minVal=3000ul)
{
    constexpr const char* name = AppOptionStr<AppOption::DURATION>::value;

    option.durationMs = defaultVal;

    if (args.containsOption(name)) {
        unsigned long inputVal = (unsigned long)(
            args.getValueForOption(name).getLargeIntValue() * 1000);
        option.durationMs = std::max(inputVal, minVal);

        args.removeValueForOption(name);
        args.removeOptionIfFound(name);
    }

}

static void parseOptionAmplitude(juce::ArgumentList& args, AppOptions& option)
{
    constexpr const char* name = AppOptionStr<AppOption::AMPLITUDE>::value;

    if (args.containsOption(name)) {
        float inputVal = (float)(
            args.getValueForOption(name).getFloatValue());

        option.amplitude = std::min(inputVal, 1.0f);
        option.amplitude = std::max(inputVal, 0.1f);

        args.removeValueForOption(name);
        args.removeOptionIfFound(name);
    }
}

static void parseOptionFrequency(juce::ArgumentList& args, AppOptions& option)
{
    constexpr const char* name = AppOptionStr<AppOption::FREQ>::value;

    if (args.containsOption(name)) {
        float inputVal = (float)(
            args.getValueForOption(name).getFloatValue());

        option.freq = std::min(inputVal, 20'000.0f);
        option.freq = std::max(inputVal, 20.0f);

        args.removeValueForOption(name);
        args.removeOptionIfFound(name);
    }
}

static void parseOptionOutputFile(juce::ArgumentList& args, AppOptions& option)
{
    constexpr const char* name = AppOptionStr<AppOption::OUT_FILE>::value;

    if (args.containsOption(name)) {
        option.outputFile = args.getValueForOption(name);

        args.removeValueForOption(name);
        args.removeOptionIfFound(name);
    }
}

static void showUnknownOptions(const juce::ArgumentList& args)
{
    if (args.size() > 0) printf("WARNING! Unknown or unused options:\n");
    for (int pos = 0; pos < args.size(); ++pos) {
        auto arg = args[pos].text;
        printf("  %s\n", arg.toRawUTF8());
    }
}

void
paf::Application::
collectOptionsForTest(const juce::ArgumentList& args_)
{
    juce::ArgumentList args(args_);

    args.removeOptionIfFound("test");

    parseOptionQuiet(args, option_);
    parseOptionDuration(args, option_);

    showUnknownOptions(args);
}

void
paf::Application::
collectOptionsForPlay(const juce::ArgumentList& args_)
{
    juce::ArgumentList args(args_);

    args.removeOptionIfFound("play");
    if (args.size() > 0 and !args[0].text.startsWith("-")) {
        args.arguments.remove(0);
    }

    parseOptionQuiet(args, option_);
    parseOptionChannels(args, option_);

    showUnknownOptions(args);
}

void
paf::Application::
collectOptionsForGenerate(const juce::ArgumentList& args_)
{
    juce::ArgumentList args(args_);

    args.removeOptionIfFound("generate");
    if (args.size() > 0 and !args[0].text.startsWith("-")) {
        args.arguments.remove(0);
    }

    parseOptionQuiet(args, option_);
    parseOptionChannels(args, option_);
    parseOptionDuration(args, option_);
    parseOptionAmplitude(args, option_);
    parseOptionFrequency(args, option_);
    parseOptionOutputFile(args, option_);

    showUnknownOptions(args);
}
