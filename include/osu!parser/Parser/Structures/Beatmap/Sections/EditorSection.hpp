#pragma once
#include <vector>
#include <iostream>
#include "Section.hpp"

namespace Parser
{
    class EditorSection : public Section
    {
    public:
        EditorSection() {}
        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);
            
            this->DistanceSpacing = this->GetAttribute("DistanceSpacing");
            this->BeatDivisor = this->GetAttribute("BeatDivisor");
            this->GridSize = this->GetAttribute("GridSize");
            this->TimelineZoom = this->GetAttribute("TimelineZoom");
        }
    public:
        std::string DistanceSpacing;
        std::string BeatDivisor;
        std::string GridSize;
        std::string TimelineZoom;
    };
}