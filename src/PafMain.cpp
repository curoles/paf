/**
 * @file
 * @brief     PAF application main entry point.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

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

int main(int argc, char* argv[])
{
    juce::ConsoleApplication cli;
 
    cli.addHelpCommand("--help|-h", "Usage:", true);
    cli.addVersionCommand("--version|-v", "PAF version 0.0.1");

    cli.addCommand({  "play",
                      "play <filename|generated-signal>",
                      "Play audio file or generated signal",
                      "Play audio file or generated signal:...",
                      [] (const auto& args) { printf("play\n"); (void)args;}});

    cli.addCommand({  "generate",
                      "generate --signal <signal-type>",
                      "Generate audio signal",
                      "Generate audio signal:\n"
                      "generate --signal <signal-type> --timeout <timeout> --file <output-file-name>",
                      [] (const auto& args) { printf("generate\n"); (void)args;}});

    cli.addCommand({  "help",
                      "help <play|generate>",
                      "Provide details for a command",
                      "Provide details for a command",
                      [&cli] (const auto& args) { showCommandDetails(cli, args);}});

    //app.addCommand ({ "--foo",
    //                  "--foo filename",
    //                  "Performs a foo operation on the given file",
    //                  [] (const auto& args) { doFoo (args); }});
 
    return cli.findAndRunCommand(
        juce::ArgumentList(argc, argv),
        /*optionMustBeFirstArg=*/true);
}

