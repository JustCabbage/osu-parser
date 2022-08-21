#pragma once

namespace Parser
{
    struct TimingPoint
    {
        std::int32_t Time;
        std::float_t BeatLength;
        std::int32_t Meter;
        std::int32_t SampleSet;
        std::int32_t SampleIndex;
        std::int32_t Volume;
        std::int32_t Uninherited;
        std::int32_t Effects;
    };
}