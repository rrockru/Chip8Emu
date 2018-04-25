#include "timer.h"

Timer::Timer()
{
    connect(this, &Timer::timeout, this, &Timer::onTimer);
    start(17);
}
