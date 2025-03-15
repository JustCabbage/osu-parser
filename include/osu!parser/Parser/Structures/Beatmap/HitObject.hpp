#pragma once
#include <optional>
#include <osu!parser/Parser/Utilities.hpp>

using Parser::Utilities::IsBitEnabled;

enum class HitObjectTypeBitmap : std::int32_t
{
    HIT_CIRCLE = 1 << 0,
    SLIDER = 1 << 1,
    SPINNER = 1 << 3,
    HOLD_NOTE = 1 << 7, // osu!mania

    NEW_COMBO = 1 << 2,
    COLOR_JUMP0 = 1 << 4,
    COLOR_JUMP1 = 1 << 5,
    COLOR_JUMP2 = 1 << 6
};

enum class HitsoundBitmap : std::uint8_t
{
    // https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#sliders:~:text=mania%20hold%20note.-,Hitsounds,-The%20hitSound%20bit

    NORMAL = 1 << 0,
    // below is additional
    WHISTLE = 1 << 1,
    FINISH = 1 << 2,
    CLAP = 1 << 3
};

namespace Parser
{
    enum class SampleSetType : std::int32_t
    {
        NO_CUSTOM = 0,
        NORMAL = 1,
        SOFT = 2,
        DRUM = 3
    };
    struct Hitsound
    {
        bool Whistle = false;
        bool Finish = false;
        bool Clap = false;

        void Import(const std::int32_t HitSound)
        {
            Whistle = IsBitEnabled(HitSound, std::int32_t(HitsoundBitmap::WHISTLE));
            Finish = IsBitEnabled(HitSound, std::int32_t(HitsoundBitmap::FINISH));
            Clap = IsBitEnabled(HitSound, std::int32_t(HitsoundBitmap::CLAP));
        }
        std::int32_t ToInt() const
        {
            std::int32_t Hitsound = std::int32_t(HitsoundBitmap::NORMAL);
            if (Whistle)
                Hitsound |= std::int32_t(HitsoundBitmap::WHISTLE);
            if (Finish)
                Hitsound |= std::int32_t(HitsoundBitmap::FINISH);
            if (Clap)
                Hitsound |= std::int32_t(HitsoundBitmap::CLAP);
            return Hitsound;
        }

        Hitsound() = default;
        Hitsound(const std::int32_t HitsoundId)
        {
            Import(HitsoundId);
        }
    };
    struct HitObjectType
    {
        bool HitCircle = false;
        bool Slider = false;
        bool Spinner = false;
        bool HoldNote = false; // osu!mania

        bool IsNewCombo = false;
        std::int32_t ComboColor = 1;

        void Import(const std::int32_t Value, const bool IsFirstNote = false, const std::int32_t OldComboColor = 1)
        {
            HitCircle = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::HIT_CIRCLE));
            Slider = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::SLIDER));
            Spinner = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::SPINNER));
            HoldNote = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::HOLD_NOTE));
            IsNewCombo = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::NEW_COMBO));

            bool bit4 = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::COLOR_JUMP0));
            bool bit5 = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::COLOR_JUMP1));
            bool bit6 = IsBitEnabled(Value, std::int32_t(HitObjectTypeBitmap::COLOR_JUMP2));
            std::int32_t ColorJump = (bit6 << 2) | (bit5 << 1) | bit4;

            if (IsFirstNote)
                ComboColor = ColorJump + 1;
            else
                ComboColor = (IsNewCombo) ? ((OldComboColor + ColorJump) % 8 + 1) : OldComboColor;
        }

        HitObjectType() = default;
        HitObjectType(const std::int32_t Value, const bool IsFirstNote = false, const std::int32_t OldComboColor = 1)
        {
            Import(Value, IsFirstNote, OldComboColor);
        }
    };

    struct HitObject
    {
        // https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#sliders:~:text=additional%20objectParams.-,Sliders,-Slider%20syntax%3A
        struct SliderParams
        {
            struct SliderCurve
            {
                enum class CurveType : char
                {
                    BEZIER = 'B',  // bezier
                    CATMULL = 'C', // centripetal catmull-rom
                    LINEAR = 'L',
                    PERFECT = 'P'
                };

                CurveType Type;
                std::vector<std::pair<std::int32_t, std::int32_t>> Points = {};

                void Import(const std::string& CurveString)
                {
                    std::vector<std::string> Curve = Utilities::Split(CurveString, '|');

                    // CurveType
                    Type = CurveType(Curve.front().front());
                    Curve.erase(Curve.begin());

                    // CurvePoints (now Curve is became CurvePoints - aka CurvePoint list)
                    for (const std::string& CurvePoint : Curve)
                    {
                        const std::vector<std::string> SplitPoint = Utilities::Split(CurvePoint, ':');
                        Points.emplace_back(std::stoi(SplitPoint[0]), std::stoi(SplitPoint[1]));
                    }
                }

                SliderCurve() = default;
                SliderCurve(const std::string& CurveString)
                {
                    Import(CurveString);
                }
            };
            struct EdgeHitsound
            {
                struct SampleSet
                {
                    SampleSetType NormalSet = SampleSetType::NO_CUSTOM;
                    SampleSetType AdditionSet = SampleSetType::NO_CUSTOM;

                    void Import(const std::string& EdgeSet)
                    {
                        if (EdgeSet.empty())
                            return; // not written
                        auto list = Utilities::Split(EdgeSet, ':');
                        NormalSet = SampleSetType(std::stoi(list[0]));
                        AdditionSet = SampleSetType(std::stoi(list[1]));
                    }

                    SampleSet() = default;
                    SampleSet(const std::string& EdgeSetStr)
                    {
                        Import(EdgeSetStr);
                    }
                };

                Hitsound Sound;   // EdgeSounds
                SampleSet Sample; // EdgeSets
            };
            struct EdgeHitsounds : std::vector<EdgeHitsound>
            {
                void Import(const std::string EdgeSoundsStr, const std::string EdgeSetsStr)
                {
                    auto EdgeSounds = Utilities::Split(EdgeSoundsStr, '|');
                    auto EdgeSets = Utilities::Split(EdgeSetsStr, '|');

                    for (std::int32_t i = 0; i < EdgeSounds.size(); i++)
                    {
                        EdgeHitsound HitSound;
                        HitSound.Sound.Import(std::stoi(EdgeSounds[i]));
                        HitSound.Sample.Import(EdgeSets[i]);

                        this->push_back(HitSound);
                    }
                }

                EdgeHitsounds() = default;
                EdgeHitsounds(const std::string EdgeSoundsStr, const std::string EdgeSetsStr)
                {
                    Import(EdgeSoundsStr, EdgeSetsStr);
                }
            };

            std::int32_t Slides = 1;  // aka Repeats
            std::double_t Length = 0; // aka PixelLength
            SliderCurve Curve;
            EdgeHitsounds edgeHitsounds; // <edgeSounds + edgeSets> list
        };
        struct HitSample
        {
            // https://osu.ppy.sh/wiki/en/Client/File_formats/osu_%28file_format%29#hitsounds:~:text=enabled%20by%20default.-,Custom%20hit%20samples,-Usage%20of%20hitSample

            SampleSetType NormalSet = SampleSetType::NO_CUSTOM;
            SampleSetType AdditionSet = SampleSetType::NO_CUSTOM;
            int Index = 0;
            int Volume = 0;
            std::string Filename;

            HitSample() = default;
            HitSample(const std::string HitSampleStr)
            {
                if (HitSampleStr.empty())
                    return; // not written
                auto list = Utilities::Split(HitSampleStr, ':');
                NormalSet = SampleSetType(std::stoi(list[0]));
                AdditionSet = SampleSetType(std::stoi(list[1]));
                Index = std::stoi(list[2]);
                Volume = std::stoi(list[3]);
                if (list.size() > 4)
                {
                    Filename = list[4];
                }
            }

            std::string GetHitsoundTypeFilename(const HitsoundBitmap HitsoundType)
            {
                if (!Filename.empty())
                {
                    // Only provide Normal hitsound
                    if (HitsoundType == HitsoundBitmap::NORMAL)
                        return Filename;
                    return "";
                }

                // sampleSet
                SampleSetType SampleSet = (HitsoundType == HitsoundBitmap::NORMAL) ? NormalSet : AdditionSet;
                std::string SampleSetStr;
                switch (SampleSet)
                {
                case SampleSetType::NORMAL:
                    SampleSetStr = "normal";
                    break;
                case SampleSetType::SOFT:
                    SampleSetStr = "soft";
                    break;
                case SampleSetType::DRUM:
                    SampleSetStr = "drum";
                    break;
                default:
                    return ""; // No custom sampleSet, use skin hitsound instand
                }

                // hitSound
                std::string HitsoundTypeStr;
                switch (HitsoundType)
                {
                case HitsoundBitmap::WHISTLE:
                    HitsoundTypeStr = "whistle";
                    break;
                case HitsoundBitmap::FINISH:
                    HitsoundTypeStr = "finish";
                    break;
                case HitsoundBitmap::CLAP:
                    HitsoundTypeStr = "clap";
                    break;
                default:
                    HitsoundTypeStr = "normal";
                    break;
                }

                std::string Filename;
                Filename.append(SampleSetStr);
                Filename.append("-hit");
                Filename.append(HitsoundTypeStr);
                if (Index != 0 && Index != 1)
                    Filename.append(std::to_string(Index));
                Filename.append(".wav");
                return Filename;
            }
        };

        std::int32_t X = 0, Y = 0;
        std::int32_t Time = 0;
        HitObjectType Type;
        Hitsound Hitsound;
        std::optional<SliderParams> SliderParameters = SliderParams();
        std::int32_t EndTime; // Bonus
        HitSample Hitsample;
    };
} // namespace Parser