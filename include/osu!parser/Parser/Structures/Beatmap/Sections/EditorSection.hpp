#pragma once
#include <vector>

#include "Section.hpp"

namespace OsuParser::Beatmap::Sections::Editor
{
    class EditorSection final : public Section
    {
    public:
        EditorSection()
        {
        }
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
} // namespace Parser