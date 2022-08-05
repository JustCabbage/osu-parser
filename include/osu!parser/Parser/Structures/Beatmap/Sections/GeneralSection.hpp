#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Section.hpp"
#include "../Utilities.hpp"

namespace Parser
{
    class GeneralSection : public Section
    {
    public:
        GeneralSection() {}
        void Parse(std::vector<std::string>& Lines) override
        {
            for(const std::string& Line : Lines)
            {
                if(Line.find(":") == std::string::npos)
                {
                    continue;
                }
                std::vector<std::string> SplitLine = Utilities::Split(Line, ':');
                if(SplitLine.size() > 1)
                {
                    this->InsertAttribute(SplitLine[0], Utilities::Trim(SplitLine[1]));
                }
            }

            this->AudioFilename = this->GetAttribute("AudioFilename");
            this->AudioLeadIn = this->GetAttribute("AudioLeadIn");
            this->PreviewTime = this->GetAttribute("PreviewTime");
            this->Countdown = this->GetAttribute("Countdown");
            this->SampleSet = this->GetAttribute("SampleSet");
            this->StackLeniency = this->GetAttribute("StackLeniency");
            this->Mode = this->GetAttribute("Mode");
            this->LetterboxInBreaks = this->GetAttribute("LetterboxInBreaks");
            this->StoryFireInFront = this->GetAttribute("StoryFireInFront");
            this->UseSkinSprites = this->GetAttribute("UseSkinSprites");
            this->AlwaysShowPlayfield = this->GetAttribute("AlwaysShowPlayfield");
            this->OverlayPosition = this->GetAttribute("OverlayPosition");
            this->SkinPreference = this->GetAttribute("SkinPreference");
            this->EpilepsyWarning = this->GetAttribute("EpilepsyWarning");
            this->CountdownOffset = this->GetAttribute("CountdownOffset");
            this->SpecialStyle = this->GetAttribute("SpecialStyle");
            this->WidescreenStoryboard = this->GetAttribute("WidescreenStoryboard");
            this->SamplesMatchPlaybackRate = this->GetAttribute("SamplesMatchPlaybackRate");
        }
    public:
        std::string AudioFilename;
        std::string AudioLeadIn;
        std::string PreviewTime;
        std::string Countdown;
        std::string SampleSet;
        std::string StackLeniency;
        std::string Mode;
        std::string LetterboxInBreaks;
        std::string StoryFireInFront;
        std::string UseSkinSprites;
        std::string AlwaysShowPlayfield;
        std::string OverlayPosition;
        std::string SkinPreference;
        std::string EpilepsyWarning;
        std::string CountdownOffset;
        std::string SpecialStyle;
        std::string WidescreenStoryboard;
        std::string SamplesMatchPlaybackRate;
    };
}