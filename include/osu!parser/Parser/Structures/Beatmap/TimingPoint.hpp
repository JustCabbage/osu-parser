#pragma once

namespace Parser
{
    struct TimingPoint
    {
        std::int32_t Time;
        std::double_t BeatLength;
        std::int32_t Meter;
        std::int32_t SampleSet;
        std::int32_t SampleIndex;
        std::int32_t Volume;
        bool Uninherited;
        std::int32_t Effects;

		bool operator< (const TimingPoint& other) const { return Time < other.Time; }
		bool operator> (const TimingPoint& other) const { return Time > other.Time; }
		bool operator== (const TimingPoint& other) const { return Time == other.Time; }
		bool operator!= (const TimingPoint& other) const { return Time != other.Time; }
		bool operator<= (const TimingPoint& other) const { return Time <= other.Time; }
		bool operator>= (const TimingPoint& other) const { return Time >= other.Time; }
    };
}