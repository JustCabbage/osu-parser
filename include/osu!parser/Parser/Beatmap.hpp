#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <format>
#include "Structures/Beatmap/TimingPoint.hpp"
#include "Structures/Beatmap/HitObject.hpp"
#include "Structures/Beatmap/Sections/GeneralSection.hpp"
#include "Structures/Beatmap/Sections/MetadataSection.hpp"
#include "Structures/Beatmap/Sections/DifficultySection.hpp"
#include "Structures/Beatmap/Sections/EditorSection.hpp"

namespace Parser
{
    class Beatmap
    {
    public:
        Beatmap(const std::string& BeatmapPath) : m_CurrentStream(BeatmapPath)
        {
            this->Reset();
            if(!m_CurrentStream.good())
            {
                return;
            }

            std::string CurrentLine = "N/A";
            std::getline(m_CurrentStream, CurrentLine);
            while(std::getline(m_CurrentStream, CurrentLine))
            {
                CurrentLine = Utilities::Trim(CurrentLine);
                static std::string CurrentSection = "General";
                if(CurrentLine.find("[") != std::string::npos && CurrentLine.find("]") != std::string::npos)
                {
                    CurrentSection = Utilities::Split(Utilities::Split(CurrentLine, '[')[1], ']')[0];
                }
                if(CurrentLine.size() < 3 || CurrentLine.find("Unicode") != std::string::npos || CurrentLine.find(CurrentSection) != std::string::npos)
                {
                    continue;
                }   
                this->m_Sections[CurrentSection].push_back(CurrentLine);
            }

            this->General.Parse(this->m_Sections["General"]);
            this->Metadata.Parse(this->m_Sections["Metadata"]);
            this->Editor.Parse(this->m_Sections["Editor"]);
            this->Difficulty.Parse(this->m_Sections["Difficulty"]);
            
            for(const std::string& ObjectString : this->m_Sections["HitObjects"])
            {
                HitObject Object;
                const std::vector<std::string> SplitObject = Utilities::Split(ObjectString, ',');
                Object.X = std::stoi(SplitObject[0]);
                Object.Y = std::stoi(SplitObject[1]);
                Object.Time = std::stoi(SplitObject[2]);
                Object.Type = HitObjectType(std::stoi(SplitObject[3]));
                Object.HitSound = HitSoundType(std::stoi(SplitObject[4]));
                switch(Object.Type)
                {
                    case HitObjectType::SLIDER:
                    {
                        std::vector<std::string> SplitCurvePoints = Utilities::Split(SplitObject[5], '|');
                        SplitCurvePoints.erase(SplitCurvePoints.begin());
                        Object.CurveType = SplitObject[5][0];
                        for(const std::string& PointString : SplitCurvePoints)
                        {
                            const std::vector<std::string> SplitPoint = Utilities::Split(PointString, ':');
                            Object.CurvePoints.push_back({ std::stoi(SplitPoint[0]), std::stoi(SplitPoint[1]) });   
                        }
                        break;
                    }
                }
                this->HitObjects.push_back(Object);
            }

            for(const std::string& PointString : this->m_Sections["TimingPoints"])
            {
                TimingPoint Point;
                const std::vector<std::string> SplitPoint = Utilities::Split(PointString, ',');
                Point.Time = std::stoi(SplitPoint[0]);
                Point.BeatLength = std::stof(SplitPoint[1]);
                Point.Meter = std::stoi(SplitPoint[2]);
                Point.SampleSet = std::stoi(SplitPoint[3]);
                Point.SampleIndex = std::stoi(SplitPoint[4]);
                Point.Volume = std::stoi(SplitPoint[5]);
                Point.Uninherited = std::stoi(SplitPoint[6]);
                Point.Effects = std::stoi(SplitPoint[7]);
                this->TimingPoints.push_back(Point);
            }
        }
    private:
        void Reset()
        {
            this->Version = 14;
            this->TimingPoints.clear();
            this->HitObjects.clear();
        }
    public:
        GeneralSection General;
        MetadataSection Metadata;
        EditorSection Editor;
        DifficultySection Difficulty;
        std::int32_t Version = 14;
        std::vector<TimingPoint> TimingPoints = {};
        std::vector<HitObject> HitObjects = {};
    private:
        std::ifstream m_CurrentStream;
        std::map<std::string, std::vector<std::string>> m_Sections = {};
    };
}