#pragma once

namespace Parser
{
    enum class HitObjectType : std::int32_t
    {
        HIT_CIRCLE = 1 << 0,
        SLIDER = 1 << 1,
        SPINNER = 1 << 3,
    };

    enum class HitSoundType : std::int32_t
    {
        NORMAL = 0,
        WHISTLE = 1,
        FINISH = 2,
        CLAP = 3
    };

    struct HitObject
    {
        std::int32_t X = 0, Y = 0;
        std::int32_t Time = 0;
        HitObjectType Type = HitObjectType::HIT_CIRCLE;
        HitSoundType HitSound = HitSoundType::NORMAL;        
        char CurveType = 'B';
        std::vector<std::pair<std::int32_t, std::int32_t>> CurvePoints = {};
        std::int32_t Repeats = 0;
        std::double_t PixelLength = 0.0;
    };
}