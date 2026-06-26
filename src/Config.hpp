#pragma once

#include "HittableList.hpp"
#include "Material.hpp"

#include <glm/glm.hpp>

#include <filesystem>
#include <memory>
#include <sstream>

namespace rt
{
    struct CameraConfig
    {
        float aspectRatio = 16.0f / 9.0f;
        int imageWidth = 1280;
        float focalLength = 1.0f;
        float viewportHeight = 2.0f;
        glm::vec3 cameraCenter = {0.0f, 0.0f, 0.0f};
    };

    struct RenderConfig
    {
        int samplesPerPixel = 10;
        int maxDepth = 5;

        std::stringstream OutputInfo() const
        {
            std::stringstream ss;

            ss << "RenderConfig:\n";
            ss << "\tSamples per pixel: " << samplesPerPixel << "\n";
            ss << "\tMax depth: " << maxDepth << "\n";

            return std::move(ss);
        }
    };

} // namespace rt
