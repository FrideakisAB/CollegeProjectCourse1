#ifndef CAMERA_H
#define CAMERA_H

#include <optional>
#include <glm/glm.hpp>
#include "GameObject.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera final : public GameObject {
public:
    constexpr static const glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    constexpr static const glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    static inline std::optional<std::reference_wrapper<Camera>> Main = std::nullopt;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) noexcept
    {
        Position = position;
        if (!Main.has_value())
            Main = std::reference_wrapper<Camera>(*this);
    }

    ~Camera() final
    {
        if (Main.has_value() && &Main.value().get() == this)
            Main = std::nullopt;
    }

    [[nodiscard]] glm::mat4 GetViewMatrix() const noexcept
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
};

#endif
