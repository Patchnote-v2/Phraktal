#ifndef TIMER_H
#define TIMER_H

#include "SDL2/SDL.h"

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void pause();
    void unpause();
    Uint32 getTicks();
    bool isStarted();
    bool isPaused();

private:
    Uint32 startedTicks;
    Uint32 pausedTicks;

    bool started;
    bool paused;
};
#endif