#pragma once
#include <vector>
#include <iostream>
#include "Section.hpp"

namespace Parser
{
    class MetadataSection : public Section
    {
    public:
        MetadataSection() {}
        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);

            this->Title = this->GetAttribute("Title");
            this->TitleUnicode = this->GetAttribute("TitleUnicode");
            this->Artist = this->GetAttribute("Artist");
            this->ArtistUnicode = this->GetAttribute("ArtistUnicode");
            this->Creator = this->GetAttribute("Creator");
            this->Version = this->GetAttribute("Version");
            this->Source = this->GetAttribute("Source");
            this->BeatmapID = this->GetAttribute("BeatmapID");
            this->BeatmapSetID = this->GetAttribute("BeatmapSetID");
        }
    public:
        std::string Title;
        std::string TitleUnicode;
        std::string Artist;
        std::string ArtistUnicode;
        std::string Creator;
        std::string Version;
        std::string Source;
        std::string BeatmapID;
        std::string BeatmapSetID;
    };
}