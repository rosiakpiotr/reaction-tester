#ifndef REACTIONTESTINGSCREEN_H
#define REACTIONTESTINGSCREEN_H

#include <TGUI/TGUI.hpp>

#include "Screen.h"
#include "TestingNode.h"
#include "AnimationPool.h"
#include "FallTowardsScreenAnimation.h"
#include "ReactionTest.h"
#include "TextList.h"

struct ReactionTestConfig : virtual ScreenParam
{
	int nodeCount = 1;
	int testCasesCount = 1;
	int difficulty = 1;
	std::vector<std::string> captions = {""};

    typedef std::shared_ptr<ReactionTestConfig> Ptr;
};

class ReactionTestingScreen : public Screen
{
public:
    ReactionTestingScreen(Resources& res): Screen(res) {}

    void prepare(std::shared_ptr<tgui::Gui> guiObject);

    void handleEvent(const sf::Event& windowEvent);

    void tick();

    const std::vector<sf::Drawable*> getDrawablesToDraw();

    void enableDrawing();
    void disableDrawing();

protected:

    void parseScreenParam(const ScreenParam::Ptr screenConfigs);

private:

    void createBackground();
    void createNodes();
    void createCasesLeftText();
    void createTimingLists();
    void attachAnimations();

    void prepareReactionTest();
    void handleReactionTest();

    void startNewTestCase();

    void updateCasesLeft(int number);
    void resetAllNodes();

    void holdUpWithNextTextFor(sf::Time howLong);
    bool celebrationsFinished();
    void addTimingToList(int listIndex, bool greenOrRed);

    TestingNode* selectNodeAt(int index);

    void setNodeStatusAtIndex(NodeStatus status, int index);

    sf::VertexArray background;
    sf::Text casesLeft;
    std::vector<TestingNode> nodes;
    std::vector<TextList> timingLists;

    std::vector<sf::Drawable*> drawables;

    ReactionTestConfig testConfig;

    sf::Clock celebrationTimer;
    sf::Time celebrationDuration;

    AnimationPool animationManager;
    ReactionTest tester;
};

#endif // REACTIONTESTINGSCREEN_H
