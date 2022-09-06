/**
 * @file
 * @brief     PAF application main entry point.
 * @author    Igor Lesik 2022
 * @copyright Igor Lesik 2022
 */
#include "JuceHeader.h"

int main(int argc, char* argv[])
{
    ConsoleApplication app;
 
    app.addHelpCommand("--help|-h", "Usage:", true);
    app.addVersionCommand("--version|-v", "PAF version 0.0.1");
 
    //app.addCommand ({ "--foo",
    //                  "--foo filename",
    //                  "Performs a foo operation on the given file",
    //                  [] (const auto& args) { doFoo (args); }});
 
    return app.findAndRunCommand(argc, argv);
}
