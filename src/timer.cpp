#include "base/timer.h"

Timer::Timer()
{
    this->startedTicks = 0;
    this->pausedTicks = 0;

    this->started = false;
    this->paused = false;
}

Timer::~Timer()
{
    //
}

void Timer::start()
{
    this->started = true;
    this->paused = false;

    this->startedTicks = SDL_GetTicks();
    this->pausedTicks = 0;
}

void Timer::stop()
{
    this->started = false;
    this->paused = false;

    this->startedTicks = 0;
    this->pausedTicks = 0;
}

void Timer::pause()
{
    if (this->started && !this->paused)
    {
        this->paused = true;

        this->pausedTicks = SDL_GetTicks() - this->startedTicks;
        this->startedTicks = 0;
    }
}

void Timer::unpause()
{
    if (this->started && this->paused)
    {
        this->paused = false;

        this->startedTicks = SDL_GetTicks() - this->pausedTicks;

        this->pausedTicks = 0;
    }
}

Uint32 Timer::getTicks()
{
    Uint32 time = 0;

    if (this->started)
    {
        if (this->paused)
        {
            time = this->pausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - this->startedTicks;
        }
    }
    return time;
}

bool Timer::isStarted()
{
    return this->started;
}

bool Timer::isPaused()
{
    return this->paused && this->started;
}