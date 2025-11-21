#pragma once
#include <vector>
#include "Structures/Beatmap/Objects/TimingPoint.hpp"
#include "Section.hpp"

namespace OsuParser::Beatmap::Sections::General
{
    enum class CountdownType : uint8_t
    {
        NO_COUNTDOWN = 0,
        NORMAL = 1,
        HALF = 2,
        DOUBLE = 3
    };

    using Objects::TimingPoint::SampleSet;

    enum class ModeType : uint8_t
    {
        OSU_STANDARD = 0,
        OSU_TAIKO = 1,
        OSU_CATCH = 2,
        OSU_MANIA = 3
    };

    enum class OverlayPositionType : uint8_t
    {
        NO_CHANGE = 0,
        BELOW = 1,
        ABOVE = 2
    };

    class GeneralSection final : public Section
    {
    public:
        GeneralSection() = default;

        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);
            this->AudioFilename = this->GetAttribute("AudioFilename");
            if (std::string audioLeadInStr = this->GetAttribute("AudioLeadIn"); !audioLeadInStr.empty()) this->AudioLeadIn = std::stoi(audioLeadInStr);
            if (std::string previewTimeStr = this->GetAttribute("PreviewTime"); !previewTimeStr.empty())
                this->PreviewTime = std::stoi(previewTimeStr);
            if (std::string countdownStr = this->GetAttribute("Countdown"); !countdownStr.empty())
            {
                switch (std::stoi(countdownStr))
                {
                case 0: this->Countdown = CountdownType::NO_COUNTDOWN;
                    break;
                case 1: this->Countdown = CountdownType::NORMAL;
                    break;
                case 2: this->Countdown = CountdownType::HALF;
                    break;
                case 3: this->Countdown = CountdownType::DOUBLE;
                    break;
                default: break;
                }
            }
            if (std::string sampleSetStr = this->GetAttribute("SampleSet"); !sampleSetStr.empty())
            {
                if (sampleSetStr == "Normal")
                {
                    this->SampleSet = SampleSet::NORMAL;
                }
                else if (sampleSetStr == "Soft")
                {
                    this->SampleSet = SampleSet::SOFT;
                }
                else if (sampleSetStr == "Drum")
                {
                    this->SampleSet = SampleSet::DRUM;
                }
            }
            if (std::string stackLeniencyStr = this->GetAttribute("StackLeniency"); !stackLeniencyStr.empty())
                this->StackLeniency = std::stod(stackLeniencyStr);
            if (std::string modeStr = this->GetAttribute("Mode"); !modeStr.empty())
            {
                switch (std::stoi(modeStr))
                {
                case 0: this->Mode = ModeType::OSU_STANDARD;
                    break;
                case 1: this->Mode = ModeType::OSU_TAIKO;
                    break;
                case 2: this->Mode = ModeType::OSU_CATCH;
                    break;
                case 3: this->Mode = ModeType::OSU_MANIA;
                    break;
                default: break;
                }
            }
            std::string letterboxInBreaksStr = this->GetAttribute("LetterboxInBreaks");
            this->LetterboxInBreaks = (!letterboxInBreaksStr.empty() && std::stoi(letterboxInBreaksStr) != 0);
            std::string useSkinSpritesStr = this->GetAttribute("UseSkinSprites");
            this->UseSkinSprites = (!useSkinSpritesStr.empty() && std::stoi(useSkinSpritesStr) != 0);
            if (std::string overlayPosStr = this->GetAttribute("OverlayPosition"); !overlayPosStr.empty())
            {
                switch (std::stoi(overlayPosStr))
                {
                case 0: this->OverlayPosition = OverlayPositionType::NO_CHANGE;
                    break;
                case 1: this->OverlayPosition = OverlayPositionType::BELOW;
                    break;
                case 2: this->OverlayPosition = OverlayPositionType::ABOVE;
                    break;
                default: break;
                }
            }
            this->SkinPreference = this->GetAttribute("SkinPreference");
            std::string epilepsyWarningStr = this->GetAttribute("EpilepsyWarning");
            this->EpilepsyWarning = (!epilepsyWarningStr.empty() && std::stoi(epilepsyWarningStr) != 0);
            if (std::string countdownOffsetStr = this->GetAttribute("CountdownOffset"); !countdownOffsetStr.empty())
                this->CountdownOffset = std::stoi(countdownOffsetStr);
            std::string specialStyleStr = this->GetAttribute("SpecialStyle");
            this->SpecialStyle = (!specialStyleStr.empty() && std::stoi(specialStyleStr) != 0);
            std::string widescreenStoryboardStr = this->GetAttribute("WidescreenStoryboard");
            this->WidescreenStoryboard = (!widescreenStoryboardStr.empty() && std::stoi(widescreenStoryboardStr) != 0);
            std::string samplesMatchPlaybackRateStr = this->GetAttribute("SamplesMatchPlaybackRate");
            this->SamplesMatchPlaybackRate = (!samplesMatchPlaybackRateStr.empty() && std::stoi(
                samplesMatchPlaybackRateStr) != 0);
        }

    public:
        std::string AudioFilename;
        int32_t AudioLeadIn = 0;
        int32_t PreviewTime = -1;
        CountdownType Countdown = CountdownType::NORMAL;
        SampleSet SampleSet = SampleSet::NORMAL;
        double_t StackLeniency = 0.7;
        ModeType Mode = ModeType::OSU_STANDARD;
        bool LetterboxInBreaks = false;
        bool UseSkinSprites = false;
        OverlayPositionType OverlayPosition = OverlayPositionType::NO_CHANGE;
        std::string SkinPreference;
        bool EpilepsyWarning = false;
        int32_t CountdownOffset = 0;
        bool SpecialStyle = false;
        bool WidescreenStoryboard = false;
        bool SamplesMatchPlaybackRate = false;
    };
} // namespace Parser