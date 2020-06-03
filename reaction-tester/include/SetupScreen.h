#pragma once

#include "Resources.h"
#include "ReactionTestingScreen.h"

class SetupScreen : public Screen
{
public:
    SetupScreen(Resources& res);

    void prepare(std::shared_ptr<tgui::Gui> guiObject);

    void handleEvent(const sf::Event& windowEvent);

    void tick();

    const std::vector<sf::Drawable*> getDrawablesToDraw();

protected:
    void parseScreenParam(const ScreenParam::Ptr screenArg);

private:
    void createAndShowBackground();
    tgui::Group::Ptr createConfigurationRow(const std::string rowCaption, int& valueToChange, int valMin, int valMax);

    // Executes when 'Start' button is pressed.
    void onStartPress();

    sf::VertexArray background;
    ReactionTestConfig config;
};