#include "RayTracer.hpp"

#include "Ray.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"

#include <stb_image_write.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>

namespace rt
{
    namespace
    {
        void writePixel(unsigned char *pixels, int pixelIndex, const glm::vec3 &color)
        {
            pixels[pixelIndex + 0] = static_cast<unsigned char>(255.999f * color.r);
            pixels[pixelIndex + 1] = static_cast<unsigned char>(255.999f * color.g);
            pixels[pixelIndex + 2] = static_cast<unsigned char>(255.999f * color.b);
        }

        glm::vec3 rayColor(const Ray &ray, const Hittable &world)
        {
            HitDesc desc;
            if (world.Hit(ray, Interval(0, std::numeric_limits<float>::infinity()), desc))
            {
                return 0.5f * (desc.normal + glm::vec3(1.0f));
            }

            glm::vec3 unitDirection = glm::normalize(ray.direction());
            float a = 0.5f * (unitDirection.y + 1.0f);
            return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
        }

    }

    void RayTracer::Render(const fs::path &output)
    {
        // Image
        float aspectRatio = 16.0f / 9.0f;
        int imageWidth = 1280;
        int imageHeight = int(imageWidth / aspectRatio);
        imageHeight = (imageHeight < 1) ? 1 : imageHeight;
        std::cout << imageWidth << ", " << imageHeight << std::endl;

        // World
        HittableList world;

        world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
        world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5, -1.0f), 100.0f));

        // Camera
        float focalLength = 1.0f;
        float viewportHeight = 2.0f;
        float viewportWidth = viewportHeight * (float(imageWidth) / imageHeight);
        glm::vec3 cameraCenter = {0, 0, 0};

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        glm::vec3 viewportU = glm::vec3(viewportWidth, 0, 0);
        glm::vec3 viewportV = glm::vec3(0, -viewportHeight, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        glm::vec3 pixelDeltaU = viewportU / static_cast<float>(imageWidth);
        glm::vec3 pixelDeltaV = viewportV / static_cast<float>(imageHeight);

        // Calculate the location of the upper left pixel.
        glm::vec3 viewportUpperLeft = cameraCenter - glm::vec3(0, 0, focalLength) - (viewportU / 2.0f) - (viewportV / 2.0f);
        glm::vec3 pixel00Loc = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

        // Allocate pixel buffer
        int channels = 3;
        int totalBytes = imageWidth * imageHeight * channels;
        unsigned char *pixels = new unsigned char[totalBytes];
        std::fill_n(pixels, totalBytes, 0);

        // Render
        for (int y = 0; y < imageHeight; ++y)
        {
            std::cout << "\rScanlines remaining: " << imageHeight - y << ' ' << std::flush;

            for (int x = 0; x < imageWidth; ++x)
            {
                int pixelIndex = (y * imageWidth + x) * channels;

                glm::vec3 pixelCenter = pixel00Loc + (static_cast<float>(x) * pixelDeltaU) + (static_cast<float>(y) * pixelDeltaV);
                glm::vec3 rayDirection = pixelCenter - cameraCenter;

                Ray r(cameraCenter, rayDirection);

                glm::vec3 pixelColor = rayColor(r, world);
                writePixel(pixels, pixelIndex, pixelColor);
            }
        }
        std::cout << "\rDone.                 \n";

        // Save
        int res = stbi_write_png(output.string().c_str(),
                                 imageWidth, imageHeight, channels,
                                 pixels,
                                 imageWidth * channels * sizeof(unsigned char));
        if (!res)
        {
            std::cerr << "Error: failed to stbi_write_png\n";
        }

        delete[] pixels;
    }

} // namespace rt
