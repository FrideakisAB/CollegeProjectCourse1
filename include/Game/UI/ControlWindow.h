#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

class ControlWindow {
private:
    bool isParametersShow = false;

public:
    static constexpr float MinTimeScale = 0.2f;
    static constexpr float MaxTimeScale = 3.f;

    void Draw();

    float Velocity = 18.f;
    float Angle = 30.f;
    float MaxHeight = 0.f;
    float Distance = 0.f;
    float TimeInFlight = 0.f;
    bool IsTrackBall = false;
    bool IsToStartRequest = false;
    bool IsToMiddleRequest = false;
    bool IsToEndRequest = false;
};

#endif
