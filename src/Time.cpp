#include "Timet.h"

Time::Time()
{

}

Time::~Time()
{
    //dtor
}

void Time::startFrame()
{
    this->currentTime = clock();
    this->dt = (this->currentTime - this->lastTime) / 1000;
    this->lastTime = this->currentTime;
}

void Time::endFrame()
{
    this->upt = true;
}

double Time::getDeltaTime()
{
    return this->dt;
}

bool Time::isLaunchWithDT()
{
    if(this->accum > 0.15f)
        this->accum = 0.15f;

    if(this->accum > fps) {
        this->accum -= fps;
        return true;
    }else{
        this->accum = this->currentTime - this->lastTime;
        return false;
    }
}
