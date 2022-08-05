#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include "Structures/Beatmap/TimingPoint.hpp"
#include "Structures/Beatmap/HitObject.hpp"
#include "Structures/Beatmap/Sections/GeneralSection.hpp"

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
        }
    private:
        void Reset()
        {
            this->Version = 14;
            this->TimingPoints.clear();
            this->HitObjects.clear();
        }
    public:
        std::int32_t Version = 14;
        GeneralSection General;
        std::vector<TimingPoint> TimingPoints = {};
        std::vector<HitObject> HitObjects = {};
    private:
        std::ifstream m_CurrentStream;
        std::map<std::string, std::vector<std::string>> m_Sections = {};
    };
}