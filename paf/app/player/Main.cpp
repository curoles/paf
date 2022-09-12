#include "MainComponent.h"

class PlayerApp : public juce::JUCEApplication
{
public:
    PlayerApp() {}

    const juce::String getApplicationName() override {
        return ProjectInfo::projectName; //JUCE_APPLICATION_NAME_STRING;
    }

    const juce::String getApplicationVersion() override {
        return ProjectInfo::versionString; //JUCE_APPLICATION_VERSION_STRING;
    }

    bool moreThanOneInstanceAllowed() override {
        return true;
    }

    void initialise(const juce::String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void anotherInstanceStarted(const juce::String& commandLine) override;

    class MainWindow : public juce::DocumentWindow
    {
    public:
        explicit MainWindow(juce::String name);

        void closeButtonPressed() override;

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (PlayerApp)

void
PlayerApp::
initialise(const juce::String& commandLine)
{
    // This method is where you should put your application's initialisation code..
    juce::ignoreUnused (commandLine);

    mainWindow.reset (new MainWindow (getApplicationName()));
}

void
PlayerApp::
shutdown()
{
    // Add your application's shutdown code here..

    mainWindow = nullptr; // destruct mainWindow and free memory
}

void
PlayerApp::
systemRequestedQuit()
{
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
}

void
PlayerApp::
anotherInstanceStarted(const juce::String& /*commandLine*/)
{
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
}

static inline
auto getDefaultBgColor() {
    return juce::Desktop::getInstance().getDefaultLookAndFeel()
        .findColour(ResizableWindow::backgroundColourId);
}

PlayerApp::MainWindow::MainWindow(juce::String name)
    : DocumentWindow (
        name,
        getDefaultBgColor(),
        DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setContentOwned(new MainComponent(), true);

   #if JUCE_IOS || JUCE_ANDROID
    setFullScreen (true);
   #else
    setResizable (true, true);
    centreWithSize (getWidth(), getHeight());
   #endif

    setVisible (true);
}

void
PlayerApp::MainWindow::
closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}
