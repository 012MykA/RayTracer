#pragma once

#include "Ray.hpp"

#include <glm/glm.hpp>

namespace rt
{
    class Camera
    {
    public:
        float aspectRatio = 16.0f / 9.0f;
        int imageWidth = 1280;
        float focalLength = 1.0f;
        float viewportHeight = 2.0f;
        glm::vec3 cameraCenter = {0.0f, 0.0f, 0.0f};

    public:
        Camera() = default;

        // Call before rendering to apply all changes
        void Initialize();

        Ray GetRay(int x, int y) const;

        Ray GetRay(int x, int y, const glm::vec2 &offset) const;

        int GetImageHeight() const { return m_ImageHeight; }

    private:
        int m_ImageHeight = 0;
        glm::vec3 m_Pixel00Loc{0.0f};
        glm::vec3 m_PixelDeltaU{0.0f};
        glm::vec3 m_PixelDeltaV{0.0f};
    };

} // namespace rt
