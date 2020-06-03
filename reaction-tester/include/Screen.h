#ifndef SCREEN_H
#define SCREEN_H

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

#include "Resources.h"

// Allows different screens pass differrent kind of informations.
struct ScreenParam
{
    virtual ~ScreenParam() {}

    std::vector<int> intParams;
    std::vector<std::string> stringParams;

    typedef std::shared_ptr<ScreenParam> Ptr;
};

class Screen
{
public:
    Screen(Resources& resources): resources(resources) {}

    virtual void prepare(std::shared_ptr<tgui::Gui> guiObject) = 0;

    virtual void handleEvent(const sf::Event& windowEvent) = 0;

    virtual void tick() = 0;

    virtual const std::vector<sf::Drawable*> getDrawablesToDraw() = 0;

    virtual std::shared_ptr<Screen> getNextScreen() { return nextScreen; }

    typedef std::shared_ptr<Screen> Ptr;

    template <class T>
    static Screen::Ptr createScreen(Resources& resources, const ScreenParam::Ptr screenParam)
    {
        Screen::Ptr screen(new T(resources));
        screen->parseScreenParam(screenParam);
        return screen;
    }

protected:

    virtual void parseScreenParam(const ScreenParam::Ptr screenConfigs) = 0;

    Resources& resources;
    Screen::Ptr nextScreen;
};

#endif // SCREEN_H
