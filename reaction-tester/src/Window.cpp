#include "Window.h"

Window::Window(Resources& res): resources(res)
{
    
}

void Window::create()
{
    windowInstance = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(constants::window::width, constants::window::height), constants::window::title);

    currentScreenGui = std::make_shared<tgui::Gui>(*windowInstance.get());
}

void Window::selectScreen(std::shared_ptr<Screen> newScreen)
{
    currentScreenGui->removeAllWidgets();
    newScreen->prepare(currentScreenGui);
    currentScreen = newScreen;
}

void Window::setFramerateLimit(unsigned int limit)
{
    windowInstance->setFramerateLimit(limit);
}

void Window::run()
{
    while(windowInstance->isOpen())
    {
        sf::Event e;
        while(windowInstance->pollEvent(e))
        {
            currentScreenGui->handleEvent(e);
            currentScreen->handleEvent(e);

            if (e.type == sf::Event::Closed)
            {
                windowInstance->close();
            }
        }

        currentScreen->tick();
        changeScreenIfReady();

        windowInstance->clear();

        const std::vector<sf::Drawable*> screenDrawables = currentScreen->getDrawablesToDraw();
        for (auto& drawable : screenDrawables)
        {
            windowInstance->draw(*drawable);
        }
        currentScreenGui->draw();
        windowInstance->display();
    }
}

void Window::changeScreenIfReady()
{
    bool isReady = currentScreen->getNextScreen() != nullptr;
    if(isReady)
    {
        selectScreen(currentScreen->getNextScreen());
    }
}
