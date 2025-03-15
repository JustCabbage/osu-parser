#pragma once
#include <osu!parser/Parser/Structures/Beatmap/HitObject.hpp>
#include <osu!parser/Parser/Utilities.hpp>

static enum class EffectBitmap : std::int32_t
{
	KIAI = 1 << 0,
	OMIT_FIRST_BARLINE = 1 << 3,
};

namespace Parser
{
    struct TimingPoint
    {
        struct Effect
        {
			bool kiai = false;
			bool omitFirstBarline = false; // osu!mania, osu!taiko

			void import(const std::int32_t effect)
			{
				kiai = isBitEnable(effect, std::int32_t(EffectBitmap::KIAI));
				omitFirstBarline = isBitEnable(effect, std::int32_t(EffectBitmap::OMIT_FIRST_BARLINE));
			}
			std::int32_t to_int() const
			{
				std::int32_t effect = 0;
				if (kiai) effect |= std::int32_t(EffectBitmap::KIAI);
				if (omitFirstBarline) effect |= std::int32_t(EffectBitmap::OMIT_FIRST_BARLINE);
				return effect;
			}

			Effect() = default;
			Effect(const std::int32_t effect) { import(effect); }
        };

        std::int32_t Time;
        std::double_t BeatLength;
        std::int32_t Meter;
        SampleSetType SampleSet;
        std::int32_t SampleIndex;
        std::int32_t Volume;
        bool Uninherited;
        Effect Effects;

		bool operator< (const TimingPoint& other) const { return Time < other.Time; }
		bool operator> (const TimingPoint& other) const { return Time > other.Time; }
		bool operator== (const TimingPoint& other) const { return Time == other.Time; }
		bool operator!= (const TimingPoint& other) const { return Time != other.Time; }
		bool operator<= (const TimingPoint& other) const { return Time <= other.Time; }
		bool operator>= (const TimingPoint& other) const { return Time >= other.Time; }
    };
}