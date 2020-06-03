#include "Resources.h"
#include "Window.h"
#include "SetupScreen.h"
#include "utility.h"

int main(int argc, char* argv[])
{
    disableSfmlConsoleOutput();

    Resources res;
    bool loadedCorrectly = res.loadAll();

    if (loadedCorrectly)
    {
        Window window(res);
        window.create();
        window.setFramerateLimit(constants::window::maxFPS);

        Screen::Ptr setupScreen = Screen::createScreen<SetupScreen>(res, nullptr);
        window.selectScreen(setupScreen);
        window.run();
    }
    else
    {
        ColorfulLogger::print(ColorfulLogger::Red, constants::message::errorLoadingResources);
    }

}
