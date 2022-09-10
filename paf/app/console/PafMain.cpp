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

    cli.addCommand({  "play",
                      "play [filename]",
                      "Play audio file",
                      "Play audio file.",
                      [&app] (const auto& args) { app.play(args); }
                      });

    cli.addCommand({  "generate",
                      "generate --signal <type>",
                      "Generate audio signal",
                      "Generate audio signal:\n"
                      "generate --signal <type> --duration <time> --file <output-file-name>",
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

    cli.addCommand ({ "--file|-f",
                      "--file|-f <filename>",
                      "File to play or write to",
                      "File to play or write to.",
                      nullptr});


    cli.addCommand ({ "--duration|-d",
                      "--duration|-d <sec>",
                      "Interrupt after number of seconds",
                      "Interrupt playing, recording or generating after number of seconds"
                      "A value of zero means infinity. The default is zero, so if this option is "
                      "omitted then the process will run until it is killed or naturally ends.",
                      nullptr});

#if 0
    app.addCommand ({ "--channels|-c",
                      "--channels|-c <number-of-channels>",
                      "The number of channels.",
                      "The number of channels. The default is one channel. Valid values are 1 through ?.",
                      [&app] (const auto& args) { app.setNrChannels(args); }});

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
    //--list-devices|l
    //--device|D
    //--list-pcms|L
    //sample format? see aplay
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

