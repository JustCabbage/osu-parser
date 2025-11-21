#pragma once
#include <vector>
#include "Section.hpp"

namespace OsuParser::Beatmap::Sections::Difficulty
{
    class DifficultySection final : public Section
    {
    public:
        DifficultySection() = default;
        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);

            const auto HPDrainRateStr = this->GetAttribute("HPDrainRate");
            const auto CircleSizeStr = this->GetAttribute("CircleSize");
            const auto OverallDifficultyStr = this->GetAttribute("OverallDifficulty");
            const auto ApproachRateStr = this->GetAttribute("ApproachRate");
            const auto SliderMultiplierStr = this->GetAttribute("SliderMultiplier");
            const auto SliderTickRateStr = this->GetAttribute("SliderTickRate");

            if (!HPDrainRateStr.empty()) HPDrainRate = std::stod(HPDrainRateStr);
            if (!CircleSizeStr.empty()) CircleSize = std::stod(CircleSizeStr);
            if (!OverallDifficultyStr.empty()) OverallDifficulty = std::stod(OverallDifficultyStr);
            if (!ApproachRateStr.empty()) ApproachRate = std::stod(ApproachRateStr);
            if (!SliderMultiplierStr.empty()) SliderMultiplier = std::stod(SliderMultiplierStr);
            if (!SliderTickRateStr.empty()) SliderTickRate = std::stod(SliderTickRateStr);
        }

        double HPDrainRate = 0;
        double CircleSize = 0;
        double OverallDifficulty = 0;
        double ApproachRate = 0;
        double SliderMultiplier = 1.4;
        double SliderTickRate = 1.0;
    };
} // namespace Parser