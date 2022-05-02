#ifndef ENGINE_H
#define ENGINE_H

#include "Timer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Render/Render.h"
#include "ResourceManager.h"
#include "Game/Simulation.h"

class Engine {
private:
    Timer timer{};
    Render render{1024, 768};
    ResourceManager resourceManager{};
    Simulation simulation{};
    GLFWwindow *window = nullptr;

    static inline Engine *instance = nullptr;

public:
    explicit Engine(GLFWwindow *window);
    ~Engine();

    void OnUpdate();
    void OnRender();
    void OnUIRender();

    void OnScroll(double yOffset);
    void OnKeyInput(int key, int action);

    static inline Engine &Get() noexcept { return *instance; }
    static inline bool IsValid() noexcept { return instance != nullptr; }

    [[nodiscard]] inline Timer &GetTimer() noexcept { return timer; }
    [[nodiscard]] inline ResourceManager &GetResourceManager() noexcept { return resourceManager; }
    [[nodiscard]] inline Render &GetRender() noexcept { return render; }
    [[nodiscard]] inline GLFWwindow *GetWindowPtr() const noexcept { return window; }
};

#endif
