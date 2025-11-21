#pragma once
#include <algorithm>
#include <optional>
#include <bitset>
#include <osu!parser/Parser/Utilities.hpp>
#include "TimingPoint.hpp"

namespace OsuParser::Beatmap::Objects::HitObject
{
    struct Point
    {
        int x;
        int y;
    };

    struct Additions
    {
        bool Normal = false;
        bool Whistle = false;
        bool Finish = false;
        bool Clap = false;

        void Import(const std::int32_t HitSound)
        {
            const auto bitmap = std::bitset<4>(HitSound);

            Normal = bitmap[0];
            Whistle = bitmap[1];
            Finish = bitmap[2];
            Clap = bitmap[3];

            if (!Normal && !Whistle && !Finish && !Clap) Normal = true;
        }

        [[nodiscard]] std::int32_t ToInt() const
        {
            auto bitmap = std::bitset<4>(0);
            bitmap[0] = Normal;
            bitmap[1] = Whistle;
            bitmap[2] = Finish;
            bitmap[3] = Clap;

            return static_cast<int32_t>(bitmap.to_ulong());
        }

        Additions() = default;
        explicit Additions(const std::int32_t& HitsoundId)
        {
            Import(HitsoundId);
        }
    };
    using TimingPoint::SampleSet;

    struct SliderSample
    {
    protected:
        static constexpr char DELIMETER = ':';

    public:
        SampleSet NormalSet = SampleSet::NO_CUSTOM;
        SampleSet AdditionSet = SampleSet::NO_CUSTOM;

        virtual void Import(const std::string& EdgeSet)
        {
            if (EdgeSet.empty())
                return; // not written
            const auto list = Utilities::Split(EdgeSet, DELIMETER);
            NormalSet = static_cast<SampleSet>(std::stoi(list[0]));
            AdditionSet = static_cast<SampleSet>(std::stoi(list[1]));
        }

        [[nodiscard]] virtual std::string ToString() const
        {
            std::string SampleStr;
            SampleStr.append(std::to_string(static_cast<std::int32_t>(NormalSet)));
            SampleStr.push_back(DELIMETER);
            SampleStr.append(std::to_string(static_cast<std::int32_t>(AdditionSet)));
            return SampleStr;
        }


        SliderSample() = default;
        virtual ~SliderSample() = default;
        explicit SliderSample(const std::string& SampleStr) { SliderSample::Import(SampleStr); }
    };
    struct HitSample final : SliderSample
    {
    protected:
        static constexpr char DELIMETER = ':';

    public:
        int Index = 0;
        int Volume = 0;
        std::string Filename{};

        void Import(const std::string& HitSampleStr) override
        {
            if (HitSampleStr.empty())
                return; // not written
            const auto list = Utilities::Split(HitSampleStr, DELIMETER);
            NormalSet = static_cast<SampleSet>(std::stoi(list[0]));
            AdditionSet = static_cast<SampleSet>(std::stoi(list[1]));
            Index = std::stoi(list[2]);
            Volume = std::stoi(list[3]);
            if (list.size() > 4) Filename = list[4];
        }

        [[nodiscard]] std::string ToString() const override
        {
            std::string SampleStr;
            SampleStr.append(std::to_string(static_cast<std::int32_t>(NormalSet)));
            SampleStr.push_back(DELIMETER);
            SampleStr.append(std::to_string(static_cast<std::int32_t>(AdditionSet)));
            SampleStr.push_back(DELIMETER);
            SampleStr.append(std::to_string(Index));
            SampleStr.push_back(DELIMETER);
            SampleStr.append(std::to_string(Volume));
            SampleStr.push_back(DELIMETER);
            if (!Filename.empty()) SampleStr.append(Filename);
            return SampleStr;
        }

        HitSample() = default;
        explicit HitSample(const std::string& HitSampleStr) { HitSample::Import(HitSampleStr); }
    };

    struct HitObject
    {
        struct Type
        {
            bool HitCircle = false;
            bool Slider = false;
            bool Spinner = false;
            bool HoldNote = false; // osu!mania
            bool IsNewCombo = false;
            std::int32_t ColourHax = 0;

            void Import(const std::int32_t Value)
            {
                const auto bitset = std::bitset<8>(Value);
                HitCircle = bitset[0];
                Slider = bitset[1];
                Spinner = bitset[3];
                HoldNote = bitset[7];
                IsNewCombo = bitset[2];

                const auto bit4 = bitset[4];
                const auto bit5 = bitset[5];
                const auto bit6 = bitset[6];
                ColourHax = bit4 | bit5 | bit6;
            }

            Type() = default;
            explicit Type(const std::int32_t Value) { Import(Value); }
        };

        struct SliderParams
        {
            struct Curve
            {
                enum class Type : char
                {
                    BEZIER = 'B', // bezier
                    CATMULL = 'C', // centripetal catmull-rom
                    LINEAR = 'L',
                    PERFECT = 'P'
                };

                Type type = Type::BEZIER;
                std::vector<Point> Points = {};

                void Import(const std::string& CurveString)
                {
                    std::vector<std::string> Curves = Utilities::Split(CurveString, '|');

                    type = static_cast<Type>(Curves.front().front());
                    Curves.erase(Curves.begin()); // Curves become CurvePoints

                    for (const std::string& CurvePoint : Curves)
                    {
                        const auto SplitPoint = Utilities::Split(CurvePoint, ':');
                        Points.emplace_back(std::stoi(SplitPoint[0]), std::stoi(SplitPoint[1]));
                    }
                }

                Curve() = default;
                explicit Curve(const std::string& CurveString) { Import(CurveString); }
            };

            std::int32_t Slides = 1; // aka Repeats
            std::double_t Length = 0; // aka PixelLength
            Curve Curve;
            std::vector<Additions> edgeSounds;
            std::vector<SliderSample> edgeSets;
        };

        Point Pos {0, 0};
        std::int32_t Time = 0;
        Type type;
        Additions Hitsound;
        std::optional<SliderParams> SliderParameters = std::nullopt;
        std::optional<double> EndTime = std::nullopt; // Bonus
        HitSample Hitsample;
    };
    struct HitObjects
    {
        std::vector<HitObject> data;
        void Parse(const std::vector<std::string>& lines, const bool sort = true)
        {
            for (const std::string& ObjectString : lines)
            {
                HitObject Object;
                const auto SplitObject = Utilities::Split(ObjectString, ',');
                Object.Pos = {std::stoi(SplitObject[0]), std::stoi(SplitObject[1])};
                Object.Time = std::stoi(SplitObject[2]);
                Object.type = HitObject::Type(std::stoi(SplitObject[3]));
                Object.Hitsound = Additions(std::stoi(SplitObject[4]));

                // Parsing objectParams
                if (Object.type.Slider)
                {
                    Object.SliderParameters.emplace();

                    Object.SliderParameters->Curve.Import(SplitObject[5]);
                    Object.SliderParameters->Slides = std::stoi(SplitObject[6]);
                    Object.SliderParameters->Length = std::stod(SplitObject[7]);
                    if (SplitObject.size() >= 10)
                    {
                        const auto EdgeSoundsStr = Utilities::Split(SplitObject[8], '|');
                        const auto EdgeSetsStr = Utilities::Split(SplitObject[9], '|');

                        for (size_t i = 0; i < EdgeSoundsStr.size(); i++)
                        {
                            Object.SliderParameters->edgeSounds.emplace_back(std::stoi(EdgeSoundsStr[i]));
                            Object.SliderParameters->edgeSets.emplace_back(EdgeSetsStr[i]);
                        }
                    }
                    while (Object.SliderParameters->edgeSounds.size() <= Object.SliderParameters->Slides)
                    {
                        Object.SliderParameters->edgeSounds.emplace_back(Object.Hitsound);
                        Object.SliderParameters->edgeSets.emplace_back(Object.Hitsample);
                    }
                }
                else if (Object.type.Spinner)
                {
                    Object.EndTime = std::stoi(SplitObject[5]);
                }

                // Parsing Hitsample
                if (Object.type.HoldNote)
                {
                    auto list = Utilities::Split(SplitObject[5], ':', true);
                    Object.EndTime = std::stoi(list.front());
                    Object.Hitsample.Import(list.back());
                }
                else {
                    if (Object.type.Slider && SplitObject.size() >= 11)
                        Object.Hitsample.Import(SplitObject[10]);
                    else if (Object.type.Spinner && SplitObject.size() >= 7)
                        Object.Hitsample.Import(SplitObject[6]);
                    else if (Object.type.HitCircle && SplitObject.size() >= 6)
                        Object.Hitsample.Import(SplitObject[5]);
                }

                data.push_back(Object);
            }

            if (sort) std::ranges::sort(data, [](const HitObject& A, const HitObject& B) { return A.Time < B.Time; });
        }
        void Parse(
            // hit object will have endTime
            const std::vector<std::string>& lines,
            const double& SliderMultiplier,
            const TimingPoint::TimingPoints& sorted_timing_points)
        {
            Parse(lines, true);

            // Get Timing Point
            std::vector<TimingPoint::TimingPoint> UninheritedTimingPoints = {};
            std::vector<TimingPoint::TimingPoint> InheritedTimingPoints = {};
            for (const auto& TimingPoint : sorted_timing_points.data)
            {
                if (TimingPoint.Uninherited)
                    UninheritedTimingPoints.push_back(TimingPoint);
                else InheritedTimingPoints.push_back(TimingPoint);
            }

            for (auto& HitObject : data)
            {
                if (!HitObject.type.HoldNote)
                {
                    if (HitObject.type.HitCircle)
                    {
                        HitObject.EndTime = HitObject.Time;
                    }
                    else if (HitObject.type.Slider && !UninheritedTimingPoints.empty())
                    {
                        TimingPoint::TimingPoint currentTimeAsTimingPoint;
                        currentTimeAsTimingPoint.Time = HitObject.Time;

                        // Get BeatLength
                        auto CurrentUninheritedTimingPoint =
                            std::ranges::lower_bound(UninheritedTimingPoints, currentTimeAsTimingPoint);
                        if (CurrentUninheritedTimingPoint == UninheritedTimingPoints.end())
                            CurrentUninheritedTimingPoint = UninheritedTimingPoints.begin();
                        const double_t BeatLength = CurrentUninheritedTimingPoint->BeatLength;

                        // Get SV
                        double_t SV;
                        auto CurrentInheritedTimingPoint =
                            std::ranges::lower_bound(InheritedTimingPoints, currentTimeAsTimingPoint);
                        if (CurrentInheritedTimingPoint == InheritedTimingPoints.end()) SV = 1;
                        else SV = 100.0 / std::abs(CurrentInheritedTimingPoint->BeatLength);

                        HitObject.EndTime =
                            static_cast<double>(HitObject.Time) + HitObject.SliderParameters->Length
                            / (SliderMultiplier * 100 * SV) * BeatLength * HitObject.SliderParameters->Slides;
                    }
                }
            }
        }
    };
} // namespace Parser
