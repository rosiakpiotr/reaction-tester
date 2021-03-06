#include "ReactionTestingScreen.h"
#include "ColorfulLogger.h"

void ReactionTestingScreen::prepare(std::shared_ptr<tgui::Gui> guiObject)
{
    createBackground();
    createNodes();
    createCasesLeftText();
    createTimingLists();

    prepareReactionTest();

    enableDrawing();

    attachAnimations();

	animationManager.setOnAnimationEnd([&](){startNewTestCase();});
    animationManager.start();
}

void ReactionTestingScreen::parseScreenParam(const ScreenParam::Ptr screenConfigs)
{
    testConfig = *std::dynamic_pointer_cast<ReactionTestConfig>(screenConfigs).get();
}

void ReactionTestingScreen::prepareReactionTest()
{
    tester = ReactionTest(
        testConfig.nodeCount,
        testConfig.testCasesCount,
        testConfig.difficulty);
}

void ReactionTestingScreen::createTimingLists()
{
    for (TestingNode& node : nodes)
    {
        TextList newTimingList(*resources.getFontRawPtr("basic"));

        sf::Vector2f nodePosition = leftTopCornerOfDrawableWithCenterOrigin(node);
        newTimingList.setPosition(sf::Vector2f(nodePosition.x, constants::window::height * 0.5));

        timingLists.push_back(newTimingList);
    }
}

void ReactionTestingScreen::createCasesLeftText()
{
    casesLeft.setFont(*resources.getFontRawPtr("basic"));
    casesLeft.setFillColor(sf::Color::White);
    casesLeft.setPosition(5, 5);

	updateCasesLeft(testConfig.testCasesCount);
}

void ReactionTestingScreen::updateCasesLeft(int number)
{
    casesLeft.setString("Cases left: " + std::to_string(number));
}

void ReactionTestingScreen::attachAnimations()
{
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        TestingNode& node = *it;

        FallTowardsScreenAnimationParam param;
        param.fromScale = 4;
        param.toScale = 1;

        Animation::Ptr nodeAnim(
            new FallTowardsScreenAnimation<TestingNode>(node, sf::milliseconds(130), param));

        animationManager.add(nodeAnim);
    }
}

void ReactionTestingScreen::createBackground()
{
	background = sf::VertexArray(sf::PrimitiveType::Quads, 4);

	background[0].position = sf::Vector2f(0, 0);
	background[1].position = sf::Vector2f(constants::window::width, 0);
	background[2].position = sf::Vector2f(constants::window::width, constants::window::height);
	background[3].position = sf::Vector2f(0, constants::window::height);

	background[0].color = sf::Color(20, 20, 25);
	background[1].color = sf::Color(15, 20, 15);
	background[2].color = sf::Color(20, 20, 20);
	background[3].color = sf::Color(20, 15, 20);
}

void ReactionTestingScreen::createNodes()
{
    int differenceDistance = constants::window::width / testConfig.nodeCount;
    int firstNodeXPos = differenceDistance / 2.f;
    int nodesYPos = constants::window::height * 0.25;

    int index = 0;
    for(const std::string& letter: testConfig.captions)
    {
        const sf::Texture* nodeTexture = resources.getTextureRawPtr("testingNode");
        const sf::Font* captionFont = resources.getFontRawPtr("testingNodeCaptionFont");

        TestingNode newNode(nodeTexture, captionFont, letter);

        sf::Vector2f nodePos(firstNodeXPos + differenceDistance * index++, nodesYPos);
        newNode.setPosition(nodePos);

        nodes.push_back(newNode);
    }
}

void ReactionTestingScreen::enableDrawing()
{
    drawables.push_back(&background);

    for (TestingNode& node: nodes)
    {
        drawables.push_back(&node);
    }

    for (TextList& timingList: timingLists)
    {
        drawables.push_back(&timingList);
    }

    drawables.push_back(&casesLeft);
}

void ReactionTestingScreen::disableDrawing()
{
    drawables.clear();
}

void ReactionTestingScreen::holdUpWithNextTextFor(sf::Time howLong)
{
    celebrationDuration = howLong;
    celebrationTimer.restart();
}

void ReactionTestingScreen::handleEvent(const sf::Event& windowEvent)
{
    if (windowEvent.type == sf::Event::KeyPressed && tester.hasTestCaseStarted())
    {
        int index = indexOfAnswerInCaptions(windowEvent.key.code, testConfig.captions);
        bool isCorrect = tester.answer(index);

        if (index > testConfig.nodeCount || index < testConfig.nodeCount)
        {
            index = tester.getRandomNodeIndex();
        }

        setNodeStatusAtIndex(isCorrect ? NodeStatus::CORRECT : NodeStatus::WRONG, index);
        updateCasesLeft(tester.casesLeftCount());

        // In this context tester.getRandomNodeIndex returns index of just selected node.
        int whichList = tester.getRandomNodeIndex();
        addTimingToList(whichList, isCorrect);

        // aka celebrations
        holdUpWithNextTextFor(sf::milliseconds(1000));
    }
}

void ReactionTestingScreen::addTimingToList(int index, bool greenOrRed)
{
    TextList& timingList = timingLists.at(index);
    std::string timingText = tester.timingToFormattedText();
    timingList.addRow(timingText, greenOrRed ? sf::Color::Green : sf::Color::Red);
}

void ReactionTestingScreen::tick()
{
    if(!animationManager.isCompleted())
    {
        animationManager.tick();
    }
    else
    {
        handleReactionTest();
    }
}

void ReactionTestingScreen::handleReactionTest()
{
    if (celebrationsFinished())
    {
        if (tester.hasTestCasesLeft())
        {
            if (tester.isTestCaseReady())
            {
                setNodeStatusAtIndex(NodeStatus::POSSIBLE, tester.getRandomNodeIndex());
            }
            else
            {
                if (!tester.hasTestCaseStarted())
                    tester.startNewCase();

                resetAllNodes();
            }
        }
        else
        {
            /* Test ended here */
            resetAllNodes();
        }


        tester.tick();
    }
}

void ReactionTestingScreen::resetAllNodes()
{
    for (auto& node : nodes)
    {
        node.setVisualStatus(NodeStatus::NEUTRAL);
    }
}

bool ReactionTestingScreen::celebrationsFinished()
{
    return celebrationTimer.getElapsedTime() >= celebrationDuration;
}

void ReactionTestingScreen::startNewTestCase()
{
    tester.startNewCase();
}

TestingNode* ReactionTestingScreen::selectNodeAt(int index)
{
    return &nodes.at(index);
}

void ReactionTestingScreen::setNodeStatusAtIndex(NodeStatus status, int index)
{
    TestingNode* node = selectNodeAt(index);
    node->setVisualStatus(status);
}

const std::vector<sf::Drawable*> ReactionTestingScreen::getDrawablesToDraw()
{
    return drawables;
}
