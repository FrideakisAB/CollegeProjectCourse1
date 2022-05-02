#ifndef ENGINE_H
#define ENGINE_H

#include "Timer.h"
#include "ResourceManager.h"

class Engine {
private:
    Timer timer{};
    ResourceManager resourceManager{};

    static inline Engine *instance = nullptr;

public:
    Engine();
    ~Engine();

    void OnUpdate();
    void OnRender();
    void OnUIRender();

    static inline Engine &Get() noexcept { return *instance; }
    static inline bool IsValid() noexcept { return instance != nullptr; }

    [[nodiscard]] inline Timer &GetTimer() noexcept { return timer; }
    [[nodiscard]] inline ResourceManager &GetResourceManager() noexcept { return resourceManager; }
};

#endif
