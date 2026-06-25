#pragma once

#include <limits>

namespace rt
{
    struct Interval
    {
        float min = +std::numeric_limits<float>::infinity();
        float max = -std::numeric_limits<float>::infinity();

        Interval() = default;
        Interval(float mn, float mx) : min(mn), max(mx) {}

        float Size() const { return max - min; }

        bool Contains(float x) const { return min <= x && x <= max; }

        bool Surrounds(float x) const { return min < x && x < max; }

        static const Interval empty, universe;
    };

} // namespace rt
