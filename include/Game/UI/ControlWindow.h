#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

class ControlWindow {
public:
    static constexpr float MinTimeScale = 0.2f;
    static constexpr float MaxTimeScale = 3.f;

    void Draw();

    float Velocity, Angle;
};

#endif
