#include "SetupScreen.h"

SetupScreen::SetupScreen(Resources& res) : Screen(res)
{

}

const std::vector<sf::Drawable*> SetupScreen::getDrawablesToDraw() 
{ 
    return { &background }; 
}

void SetupScreen::prepare(std::shared_ptr<tgui::Gui> guiObject)
{
    createAndShowBackground();

    auto rows = {
        createConfigurationRow("Nodes count", config.nodeCount, 1, 5),
        createConfigurationRow("Test case count", config.testCasesCount, 1, 5),
        createConfigurationRow("Difficulty", config.difficulty, 1, 5)
    };

    int i = 0;
    for (auto& row : rows)
    {
        row->setPosition("", constants::window::height * 0.10 + (50 * i++));
        guiObject->add(row);
    }

    auto start = tgui::Button::create("Start");
    start->setSize(110, 50);
    start->setPosition("(0.5 * parent.width) - (width / 2.f)", "0.8 * parent.height");
    start->connect("pressed", &SetupScreen::onStartPress, this);
    guiObject->add(start);
}

void SetupScreen::onStartPress()
{
    config.captions = {};

    const std::vector<std::string> predefinedKeys = { "Q", "W", "E", "R", "T" };

    for (int i = 0; i < config.nodeCount; i++)
    {
        config.captions.push_back(predefinedKeys.at(i));
    }

    nextScreen = Screen::createScreen<ReactionTestingScreen>(resources, std::make_shared<ReactionTestConfig>(config));
}

tgui::Group::Ptr SetupScreen::createConfigurationRow(const std::string rowCaption, int& valueToChange, int valMin, int valMax)
{
    tgui::Group::Ptr row = tgui::Group::create();

    auto caption = tgui::Label::create(rowCaption);
    caption->setPosition("30%", "parent.y");
    caption->getRenderer()->setTextColor(tgui::Color::White);
    row->add(caption);

    auto value = tgui::Label::create(std::to_string(valueToChange));
    value->setPosition("60%", "parent.y");
    value->getRenderer()->setTextColor(tgui::Color::White);
    row->add(value);

    auto decrease = tgui::Button::create("-");
    decrease->setPosition("62%", "parent.y");
    decrease->setSize(20, tgui::bindHeight(value));
    row->add(decrease);

    auto increase = tgui::Button::create("+");
    increase->setPosition("65%", "parent.y");
    increase->setSize(20, tgui::bindHeight(value));
    row->add(increase);

    decrease->connect("pressed",
        [label = value, val = std::ref(valueToChange), valMin]() {
        if (val > valMin)
            label->setText(std::to_string(--val));
    });

    increase->connect("pressed",
        [label = value, val = std::ref(valueToChange), valMax]() {
        if (val < valMax)
            label->setText(std::to_string(++val));
    });

    return row;
}

void SetupScreen::createAndShowBackground()
{
    background = sf::VertexArray(sf::PrimitiveType::Quads, 4);

    background[0].position = sf::Vector2f(0, 0);
    background[1].position = sf::Vector2f(constants::window::width, 0);
    background[2].position = sf::Vector2f(constants::window::width, constants::window::height);
    background[3].position = sf::Vector2f(0, constants::window::height);

    background[0].color = sf::Color(50, 50, 50);
    background[1].color = sf::Color(45, 42, 45);
    background[2].color = sf::Color(52, 51, 50);
    background[3].color = sf::Color(50, 50, 47);
}

void SetupScreen::handleEvent(const sf::Event& windowEvent) 
{ 

}

void SetupScreen::tick() 
{ 

}

void SetupScreen::parseScreenParam(const ScreenParam::Ptr screenArg) 
{

}