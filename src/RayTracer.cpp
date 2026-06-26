#include "RayTracer.hpp"

#include "Sphere.hpp"
#include "Material.hpp"
#include "Random.hpp"
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
#include <atomic>

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

        // Camera
        m_Camera.Initialize();

        // Image
        int width = m_Camera.imageWidth;
        int height = m_Camera.GetImageHeight();
        const int channels = 3;

        std::cout << "Rendering image: " << width << "x" << height << "\n";

        // Allocate pixel buffer
        int totalBytes = width * height * channels;
        auto *pixels = new unsigned char[totalBytes];
        std::fill_n(pixels, totalBytes, 0);

        float pixelSamplesScale = 1.0f / m_RenderConfig.samplesPerPixel;

        Timer renderTimer;

        // clang-format off
        
        auto rows = std::views::iota(0, height);
        std::atomic<int> completeRows{0};
        std::for_each(std::execution::seq, rows.begin(), rows.end(), [&](int y)
        {
            Timer rowTimer;

            for (int x = 0; x < width; ++x)
            {
                glm::vec3 pixelColor{0.0f};
                int pixelIndex = (y * width + x) * channels;

                for (int sample = 0; sample < m_RenderConfig.samplesPerPixel; ++sample)
                {
                    glm::vec2 offset = getRandomOffset();
                    Ray r = m_Camera.GetRay(x, y, offset);

                    pixelColor += rayColor(r, m_RenderConfig.maxDepth, m_World);
                }
                pixelColor *= pixelSamplesScale;
                writePixel(pixels, pixelIndex, pixelColor);
            }
            double elapsed = rowTimer.ElapsedSeconds();
            
            int done = ++completeRows;
            int percent = (done * 100) / height;
            double left = elapsed * (height - done);
            
            std::cout << "\rProgress: " << percent << "% (" << left << " s)" << std::flush;
        });
        // clang-format on
        std::cout << "\rDone (" << renderTimer.ElapsedSeconds() << "s)" << "                 \n";

        try
        {
            std::ofstream fs("images/result.txt");

            if (fs.is_open())
            {
                fs << "Image:" << "\n";
                fs << "\tWidth: " << width << "\n";
                fs << "\tHeight: " << height << "\n\n";

                fs << m_RenderConfig.OutputInfo().str();

                fs << "\nRender time: " << renderTimer.ElapsedMilliseconds() << " ms" << "\n";
            }
            fs.close();
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
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
