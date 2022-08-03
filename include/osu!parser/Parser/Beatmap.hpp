#pragma once
#include <string>
#include <vector>
#include "Structures/TimingPoint.hpp"
#include "Structures/HitObject.hpp"

namespace Parser
{
    class Beatmap
    {
    public:
        Beatmap(const std::string& BeatmapPath) 
        {
        }
    public:
        std::int32_t Version = 14;
        std::vector<TimingPoint> TimingPoints = {};
        std::vector<HitObject> HitObjects = {};
    };
}