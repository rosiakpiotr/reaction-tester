#include "Animation.h"

Animation::Animation() : targetDuration(sf::Time::Zero), isAnimationFinished(false)
{
}

Animation::Animation(const sf::Time duration) : targetDuration(duration), isAnimationFinished(false)
{
}

bool Animation::isCompleted()
{
    return elapsedSinceStart >= targetDuration;;
}

void Animation::start()
{
    timer.restart();
}

float Animation::getProgressPercentage()
{
    elapsedSinceStart = timer.getElapsedTime();
    return elapsedSinceStart / targetDuration;
}
