#ifndef TIME_H
#define TIME_H

#include <ctime>

class Time
{
public:
    Time();
    virtual ~Time();

    void startFrame();
    void endFrame();
    double getDeltaTime();
    bool isLaunchWithDT();
private:
    double currentTime = 0, lastTime = 0, accum = 0;
    double dt =1, dt1 = 0, dt2 = 0;
    double fps = 1 / 45;
    bool upt = false;
};

#endif // TIME_H
