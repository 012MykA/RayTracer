#pragma once

#include <filesystem>

namespace rt
{
    namespace fs = std::filesystem;
    
    class RayTracer
    {
    public:
        void Render(const fs::path &output);

    private:
    };

} // namespace rt
