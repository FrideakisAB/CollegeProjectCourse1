#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <memory>
#include "Game/Sprite.h"
#include "Game/Camera.h"
#include "Game/TextSprite.h"
#include "Game/UI/ControlWindow.h"

class Simulation {
private:
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float g = 9.8066f;
    static constexpr float halfG = g / 2.f;
    static constexpr float yScale = 1.f;
    static constexpr float xScale = 1.f;
    static constexpr float cameraSpeed = 80.f;
    static constexpr float cameraShiftSpeed = 2.5f;

    static constexpr float zoomSens = 0.03f;
    static constexpr float zoomMin = 0.2f;
    static constexpr float zoomMax = 3.f;

    ControlWindow controlWindow{};

    bool isSimulationActive = false;
    float startSpeed = 18;
    float angle = 30;
    float sinAngle = sin(angle * PI / 180);
    float cosAngle = cos(angle * PI / 180);

    float simulationTime = 0.f;
    float simulationEndTime = (startSpeed / halfG) * sinAngle;

    float throwDistance = startSpeed * cosAngle * simulationEndTime;
    float maxHeight = startSpeed * sinAngle * 0.5f * simulationEndTime - halfG * 0.25f * simulationEndTime * simulationEndTime;

    float x, y;

    std::vector<glm::vec2> points;

    std::unique_ptr<Camera> camera{};

    std::unique_ptr<Sprite> ball{};
    std::unique_ptr<Sprite> falseBall{};
    std::unique_ptr<Sprite> arrowXBody{};
    std::unique_ptr<Sprite> arrowYBody{};
    std::unique_ptr<Sprite> arrowXEnd{};
    std::unique_ptr<Sprite> arrowYEnd{};
    std::unique_ptr<Sprite> arrowStart{};

    std::unique_ptr<TextSprite> textY{};
    std::unique_ptr<TextSprite> textX{};

    static void CameraControl();
    void ParseInput();
    void CalculateFromValues();
    void RenderOnUI() const;
    void SetTitleFromState() const;
    void SimulationIterate();

public:
    Simulation();

    void PostInit();

    void OnUpdate();
    void OnRender();
    void OnUIRender();

    void OnScroll(double yOffset);
    void OnKeyInput(int key, int action);
};

#endif
