#pragma once
#include <algorithm>
#include <bitset>
#include <osu!parser/Parser/Utilities.hpp>

namespace OsuParser::Beatmap::Objects::TimingPoint
{
    enum class SampleSet : std::uint8_t
    {
        NO_CUSTOM = 0,
        NORMAL = 1,
        SOFT = 2,
        DRUM = 3
    };

    struct TimingPoint
    {
        struct Effect
        {
            bool kiai = false;
            bool omitFirstBarline = false; // osu!mania, osu!taiko

            void Import(const std::int32_t effect)
            {
                const auto bitmap = std::bitset<8>(effect);

                kiai = bitmap[0];
                omitFirstBarline = bitmap[3];
            }
            [[nodiscard]] std::int32_t to_int() const
            {
                auto bitmap = std::bitset<8>(0);

                bitmap[0] = kiai;
                bitmap[3] = omitFirstBarline;

                return static_cast<int32_t>(bitmap.to_ulong());
            }

            Effect() = default;
            explicit Effect(const std::int32_t effect) { Import(effect); }
        };

        std::int32_t Time{};
        std::double_t BeatLength{};
        std::int32_t Meter{};
        SampleSet SampleSet{};
        std::int32_t SampleIndex{};
        std::int32_t Volume{};
        bool Uninherited{};
        Effect Effects{};

        bool operator<(const TimingPoint& other) const { return Time < other.Time; }
        bool operator>(const TimingPoint& other) const { return Time > other.Time; }
        bool operator==(const TimingPoint& other) const { return Time == other.Time; }
        bool operator!=(const TimingPoint& other) const { return Time != other.Time; }
        bool operator<=(const TimingPoint& other) const { return Time <= other.Time; }
        bool operator>=(const TimingPoint& other) const { return Time >= other.Time; }
    };

    struct TimingPoints
    {
        std::vector<TimingPoint> data{};
        void Parse(const std::vector<std::string>& lines, const bool sort = true)
        {
            for (const auto& line : lines)
            {
                TimingPoint point;
                const auto split = Utilities::Split(line, ',');
                point.Time = std::stoi(split[0]);
                point.BeatLength = std::stod(split[1]);
                point.Meter = std::stoi(split[2]);
                point.SampleSet = static_cast<SampleSet>(std::stoi(split[3]));
                point.SampleIndex = std::stoi(split[4]);
                point.Volume = std::stoi(split[5]);
                point.Uninherited = (std::stoi(split[6]) == 1);
                if (split.size() >= 8) point.Effects.Import(std::stoi(split[7]));
                data.push_back(std::move(point));
            }
            if (sort) std::ranges::sort(data);
        }
    };
}
