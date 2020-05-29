#include "Resources.h"
#include "Window.h"
#include "ReactionTestingScreen.h"
#include "utility.h"

class SetupScreen : public Screen
{
public:

    SetupScreen(Resources& res) : Screen(res)
    {

    }

    void prepare(std::shared_ptr<tgui::Gui> guiObject)
    {
        createAndAddVisualConfigRow(guiObject, nodesCountDisplayText, config.nodeCount, constants::window::height * 0.15);
        createAndAddVisualConfigRow(guiObject, testCasesCountDisplayText, config.testCasesCount, constants::window::height * 0.25);
        createAndAddVisualConfigRow(guiObject, difficultyDisplayText, config.difficulty, constants::window::height * 0.35);

        config.captions = { "Q", "W", "E", "R" };

        auto start = tgui::Button::create("Start");
        start->setSize(110, 50);
        start->setPosition("(0.5 * parent.width) - (width / 2.f)", "0.8 * parent.height");
        start->connect("pressed", [&] { nextScreen = Screen::createScreen<ReactionTestingScreen>(resources, std::make_shared<ReactionTestConfig>(config)); });
        guiObject->add(start);
    }

    void handleEvent(const sf::Event& windowEvent) { }

    void tick() { }

    const std::vector<sf::Drawable*> getDrawablesToDraw()
    {
        return {};
    }

protected:
    void parseScreenParam(const ScreenParam::Ptr screenArg) {}

private:
    void createAndAddVisualConfigRow(std::shared_ptr<tgui::Gui> guiObject, const std::string rowCaption, int& valueToChange, int positionY)
    {
        auto caption = tgui::Label::create(rowCaption);
        caption->setWidgetName(rowCaption);
        caption->setPosition("30%", positionY);
        guiObject->add(caption);

        auto value = tgui::Label::create(std::to_string(valueToChange));
        value->setWidgetName("value");
        value->setPosition("60%", positionY);
        guiObject->add(value);

        auto decrease = tgui::Button::create("-");
        decrease->setPosition("62%", positionY);
        decrease->connect("pressed", &SetupScreen::onDecreasePress, this, value, std::ref(valueToChange));
        guiObject->add(decrease);

        auto increase = tgui::Button::create("+");
        increase->setPosition("65%", positionY);
        increase->connect("pressed", &SetupScreen::onIncreasePress, this, value, std::ref(valueToChange));
        guiObject->add(increase);
    }

    void onDecreasePress(tgui::Label::Ptr labelToUpdate, int& value)
    { 
        updateLabel(labelToUpdate, std::to_string(--value));
    }

    void onIncreasePress(tgui::Label::Ptr labelToUpdate, int& value)
    {
        updateLabel(labelToUpdate, std::to_string(++value));
    }

    void updateLabel(tgui::Label::Ptr label, std::string text)
    {
        label->setText(text);
    }

    const std::string nodesCountDisplayText = "Nodes count";
    const std::string testCasesCountDisplayText = "Test case count";
    const std::string difficultyDisplayText = "Difficulty";

    ReactionTestConfig config;
};

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
