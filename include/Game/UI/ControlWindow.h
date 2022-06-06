#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

class ControlWindow {
public:
    static constexpr float MinTimeScale = 0.2f;
    static constexpr float MaxTimeScale = 3.f;

    void Draw();

    float Velocity = 18.f;
    float Angle = 30.f;
    bool IsTrackBall = false;
    bool IsToStartRequest = false;
    bool IsToMiddleRequest = false;
    bool IsToEndRequest = false;
};

#endif
