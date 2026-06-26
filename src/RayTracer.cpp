#include "RayTracer.hpp"

#include "Ray.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Random.hpp"
#include "Material.hpp"
#include "Timer.hpp"

#include <stb_image_write.h>
#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <execution>
#include <ranges>
#include <fstream>

namespace rt
{
    namespace
    {
        void writePixel(unsigned char *pixels, int pixelIndex, const glm::vec3 &color)
        {
            glm::vec3 gamma = glm::sqrt(color);

            pixels[pixelIndex + 0] = static_cast<unsigned char>(256 * std::clamp(gamma.r, 0.0f, 0.999f));
            pixels[pixelIndex + 1] = static_cast<unsigned char>(256 * std::clamp(gamma.g, 0.0f, 0.999f));
            pixels[pixelIndex + 2] = static_cast<unsigned char>(256 * std::clamp(gamma.b, 0.0f, 0.999f));
        }

        glm::vec3 rayColor(const Ray &ray, int depth, const Hittable &world)
        {
            if (depth <= 0)
                return glm::vec3(0.0f);

            HitDesc desc;
            if (world.Hit(ray, Interval(0.001f, std::numeric_limits<float>::infinity()), desc))
            {
                Ray scattered;
                glm::vec3 attenuation;
                if (desc.material->Scatter(ray, desc, attenuation, scattered))
                {
                    return attenuation * rayColor(scattered, depth - 1, world);
                }

                return glm::vec3(0.0f);
            }

            glm::vec3 unitDirection = glm::normalize(ray.direction());
            float a = 0.5f * (unitDirection.y + 1.0f);
            return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
        }

        glm::vec2 getRandomOffset()
        {
            return glm::vec2(randomFloat(-0.5f, 0.5f), randomFloat(-0.5f, 0.5f));
        }

    }

    void RayTracer::Render(const fs::path &output)
    {
        // World
        HittableList world;

        auto materialGround = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
        auto materialCenter = std::make_shared<Lambertian>(glm::vec3(0.1, 0.2, 0.5));
        auto materialLeft = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3f);
        auto materialRight = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f);

        world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
        world.Add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.2f), 0.5f, materialCenter));
        world.Add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, materialLeft));
        world.Add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, materialRight));

        // Camera
        Camera camera;
        camera.Initialize();

        // Image
        int width = camera.imageWidth;
        int height = camera.GetImageHeight();
        const int channels = 3;

        std::cout << "Rendering image: " << width << "x" << height << std::endl;

        // Allocate pixel buffer
        int totalBytes = width * height * channels;
        auto *pixels = new unsigned char[totalBytes];
        std::fill_n(pixels, totalBytes, 0);

        // Render
        int samplesPerPixel = 100; // Setting
        int maxDepth = 50;         // Setting

        float pixelSamplesScale = 1.0f / samplesPerPixel;

        Timer renderTimer;

#define MULTITHREAD_RENDER 1

#if MULTITHREAD_RENDER
        auto rows = std::views::iota(0, height);

        // clang-format off
        std::for_each(std::execution::par, rows.begin(), rows.end(), [&](int y)
        {
            for (int x = 0; x < width; ++x)
            {
                glm::vec3 pixelColor{0.0f};
                int pixelIndex = (y * width + x) * channels;

                for (int sample = 0; sample < samplesPerPixel; ++sample)
                {
                    glm::vec2 offset = getRandomOffset();
                    Ray r = camera.GetRay(x, y, offset);

                    pixelColor += rayColor(r, maxDepth, world);
                }
                pixelColor *= pixelSamplesScale;
                writePixel(pixels, pixelIndex, pixelColor);
            }
        });
        // clang-format on

#else
        for (int y = 0; y < height; ++y)
        {
            std::cout << "\rScanlines remaining: " << height - y << ' ' << std::flush;

            for (int x = 0; x < width; ++x)
            {
                glm::vec3 pixelColor{0.0f};
                int pixelIndex = (y * width + x) * channels;

                for (int sample = 0; sample < samplesPerPixel; ++sample)
                {
                    glm::vec2 offset = getRandomOffset();
                    Ray r = camera.GetRay(x, y, offset);

                    pixelColor += rayColor(r, maxDepth, world);
                }
                pixelColor *= pixelSamplesScale;
                writePixel(pixels, pixelIndex, pixelColor);
            }
        }
#endif
        std::cout << "\rDone (" << renderTimer.ElapsedSeconds() << "s)" << "                 \n";

        try
        {
            std::ofstream fs(output.string() + ".txt");

            if (fs.is_open())
            {
                fs << "Image:" << std::endl;
                fs << "\tWidth: " << width << std::endl;
                fs << "\tHeight: " << height << std::endl;
                fs << "Render time: " << renderTimer.ElapsedMilliseconds() << " ms" << std::endl;
            }
            fs.close();
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        // Save
        int res = stbi_write_png(output.string().c_str(),
                                 width, height, channels,
                                 pixels,
                                 width * channels * sizeof(unsigned char));
        if (!res)
        {
            std::cerr << "Error: failed to stbi_write_png\n";
        }

        delete[] pixels;
    }

} // namespace rt
