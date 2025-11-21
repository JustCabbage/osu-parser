#pragma once
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

#include "Structures/Beatmap/Sections/DifficultySection.hpp"
#include "Structures/Beatmap/Sections/EditorSection.hpp"
#include "Structures/Beatmap/Sections/GeneralSection.hpp"
#include "Structures/Beatmap/Sections/MetadataSection.hpp"
#include "Structures/Beatmap/Sections/ColourSection.hpp"
#include "Structures/Beatmap/Sections/VariableSection.hpp"
#include "Structures/Beatmap/Objects/HitObject.hpp"
#include "Structures/Beatmap/Objects/TimingPoint.hpp"
#include "Structures/Beatmap/Objects/Event.hpp"

namespace OsuParser::Beatmap
{
    static constexpr int MINIMUM_LINE_CHARACTERS = 3;

    class Beatmap
    {
    public:
        explicit Beatmap(const std::string& BeatmapPath, const bool OnlyEvents = false) : m_CurrentStream(BeatmapPath)
        {
            this->Reset();
            if (!m_CurrentStream.good()) return;

            std::string CurrentLine;
            std::string CurrentSection = {};
            while (std::getline(m_CurrentStream, CurrentLine))
            {
                if (CurrentSection == "Events")
                {
                    CurrentLine = Utilities::Trim(CurrentLine, true);
                    const auto CurrentLineIfAllTrimmed = Utilities::Trim(CurrentLine);

                    // minimum chars
                    if (CurrentLineIfAllTrimmed.size() < MINIMUM_LINE_CHARACTERS)
                        continue;

                    // is comment
                    if (CurrentLineIfAllTrimmed[0] == '/' && CurrentLineIfAllTrimmed[1] == '/')
                        continue;
                }
                else
                {
                    CurrentLine = Utilities::Trim(CurrentLine);
                    if (CurrentLine.size() < MINIMUM_LINE_CHARACTERS)
                        continue;
                    if (CurrentLine.size() >= 2 && CurrentLine[0] == '/' && CurrentLine[1] == '/') // is comment
                        continue;
                }

                // is section
                if (!CurrentLine.empty() && CurrentLine.front() == '[' && CurrentLine.back() == ']')
                {
                    CurrentSection = Utilities::Split(Utilities::Split(CurrentLine, '[')[1], ']')[0];
                    continue;
                }


                this->m_Sections[CurrentSection].push_back(CurrentLine);
            }

            if (!OnlyEvents)
            {
                // Sections
                this->General.Parse(this->m_Sections["General"]);
                this->Metadata.Parse(this->m_Sections["Metadata"]);
                this->Editor.Parse(this->m_Sections["Editor"]);
                this->Difficulty.Parse(this->m_Sections["Difficulty"]);
                this->Colours.Parse(this->m_Sections["Colours"]);

                // Objects
                this->TimingPoints.Parse(this->m_Sections["TimingPoints"], !this->m_Sections["HitObjects"].empty());
                if (!TimingPoints.data.empty())
                    this->HitObjects.Parse(this->m_Sections["HitObjects"],
                                           this->Difficulty.SliderMultiplier, this->TimingPoints);
                else this->HitObjects.Parse(this->m_Sections["HitObjects"]);
            }
            this->Variables.Parse(this->m_Sections["Variables"]);
            this->Events.Parse(this->m_Sections["Events"], this->Variables);
        }

    private:
        void Reset()
        {
            TimingPoints.data.clear();
            HitObjects.data.clear();
            Events.objects.clear();
        }

    public:
        std::int32_t Version = 14;
        // Sections
        Sections::General::GeneralSection General;
        Sections::Metadata::MetadataSection Metadata;
        Sections::Editor::EditorSection Editor;
        Sections::Difficulty::DifficultySection Difficulty;
        Sections::Colour::ColourSection Colours;
        Sections::Variable::VariableSection Variables;
        // Objects
        Objects::TimingPoint::TimingPoints TimingPoints;
        Objects::HitObject::HitObjects HitObjects;
        Objects::Event::Events Events;

    private:
        using Sections = std::unordered_map<std::string, std::vector<std::string>>;

        std::ifstream m_CurrentStream;
        Sections m_Sections = {};
    };
}
