#pragma once
#include <vector>
#include <iostream>
#include "Section.hpp"

namespace Parser
{
    class DifficultySection : public Section
    {
    public:
        DifficultySection() {}
        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);
            
            this->HPDrainRate = this->GetAttribute("HPDrainRate");
            this->CircleSize = this->GetAttribute("CircleSize");
            this->OverallDifficulty = this->GetAttribute("OverallDifficulty");
            this->ApproachRate = this->GetAttribute("ApproachRate");
            this->SliderMultiplier = this->GetAttribute("SliderMultiplier");
            this->SliderTickRate = this->GetAttribute("SliderTickRate");
        }
    public:
        std::string HPDrainRate;
        std::string CircleSize;
        std::string OverallDifficulty;
        std::string ApproachRate;
        std::string SliderMultiplier;
        std::string SliderTickRate;
    };
}