/**
 * @file
 * @brief     PAF application main entry point.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

#include "PafApp.h"

static
void showCommandDetails(
    const juce::ConsoleApplication& cli,
    const juce::ArgumentList& args
)
{
    if (args.size() > 1) {
        const auto* cmd = cli.findCommand(juce::ArgumentList(String(), args[1].text), true);
        if (cmd) {
            cli.printCommandDetails(args, *cmd);
        }
        else {
            printf("Can't find %s\nUse: help <type|generate>\n", args[1].text.toRawUTF8());
        }
    }
    else {
        printf("Use: help <type|generate>\n");
    }
}

static
void defineCommandLineOptions(
    juce::ConsoleApplication& cli,
    paf::Application& app
)
{
    cli.addHelpCommand("--help|-h", "Usage:", true);
    cli.addVersionCommand("--version|-v", "PAF version 0.0.1");

    cli.addCommand({  "test",
                      "test",
                      "Test sound",
                      "Play a beep through the current audio device. "
                      "Use --duration|-d to provide time.",
                      [&app] (const auto& args) { app.test(args); }
                      });

    cli.addCommand({  "list",
                      "list",
                      "List audio drivers and devices",
                      "List discovered system audio drivers and devices",
                      [&app] (const auto& args) { app.list(args); }
                      });

    cli.addCommand({  "play",
                      "play <filename>",
                      "Play audio file",
                      "Play audio file.",
                      [&app] (const auto& args) { app.play(args); }
                      });

    cli.addCommand({  "generate",
                      "generate <type>",
                      "Generate audio signal",
                      "Generate audio signal.",
                      [&app] (const auto& args) { app.generate(args); }
                      });

    //cli.addCommand({  "record",

    cli.addCommand({  "help",
                      "help <play|generate>",
                      "Provide details for a command",
                      "Provide details for a command",
                      [&cli] (const auto& args) { showCommandDetails(cli, args); }
                      });

    cli.addCommand ({ "--quiet|-q",
                      "--quiet|-q",
                      "Quiet mode, no diagnostic messages",
                      "Do not show diagnostic messages.",
                      nullptr});

    cli.addCommand ({ "--channels|-c",
                      "--channels|-s <number>",
                      "Number of audio channels, default is 2",
                      "Define number of audio channels to use, default is 2.",
                      nullptr});

    cli.addCommand ({ "--amplitude|-a",
                      "--amplitude|-a <value>",
                      "Signal amplitude",
                      "Amplitude of generated signal.",
                      nullptr});

    cli.addCommand ({ "--freq|-f",
                      "--freq|-f <valueHz>",
                      "Signal frequency",
                      "Frequency of generated signal.",
                      nullptr});

    cli.addCommand ({ "--duration|-d",
                      "--duration|-d <sec>",
                      "Interrupt after number of seconds",
                      "Interrupt playing, recording or generating after number of seconds"
                      "A value of zero means infinity. The default is zero, so if this option is "
                      "omitted then the process will run until it is killed or naturally ends.",
                      nullptr});

    cli.addCommand ({ "--output-file|-o",
                      "--output-file|-o <filename>",
                      "Output file",
                      "output file to write to.",
                      nullptr});
#if 0
    app.addCommand ({ "--rate|-r",
                      "--rate|-r <sampling-rate-in-Hz>",
                      "Sampling rate in Hz.",
                      "Sampling rate in Hz.",
                      [&app] (const auto& args) { app.setSamplingRate(args); }});

    app.addCommand ({ "--file-type|-t",
                      "--file-type|-t <file-type>",
                      "File type TODO LIST.",
                      "File type TODO LIST. Default is ?. "
                      "File extention is used by default.",
                      [&app] (const auto& args) { app.setSamplingRate(args); }});

    //--gain|g
    //--device|D
#endif

}

int main(int argc, char* argv[])
{
    paf::Application app;

    juce::ConsoleApplication cli;
 
    defineCommandLineOptions(cli, app);

    return cli.findAndRunCommand(
        juce::ArgumentList(argc, argv),
        /*optionMustBeFirstArg=*/true);
}

