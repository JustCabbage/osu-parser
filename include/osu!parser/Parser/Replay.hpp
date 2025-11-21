#pragma once
#define POCKETLZMA_LZMA_C_DEFINE
#include <string>
#include <vector>
#include "Utilities.hpp"
#include "Reader/Reader.hpp"
#include "Structures/Replay/ReplayAction.hpp"
#include "Dependencies/pocketlzma/pocketlzma.hpp"

namespace OsuParser
{
    class Replay
    {
    public:
        Replay(const std::string& ReplayPath)
        {
            this->Reset();
            if(!m_Reader.SetStream(ReplayPath))
            {
                return;
            }

            this->ReplayMode = this->m_Reader.ReadType<std::uint8_t>();
            this->Version = this->m_Reader.ReadType<std::uint32_t>();
            this->BeatmapHash = this->m_Reader.ReadString();
            this->PlayerName = this->m_Reader.ReadString();
            this->ReplayHash = this->m_Reader.ReadString();
            this->Count300 = this->m_Reader.ReadType<std::uint16_t>();
            this->Count100 = this->m_Reader.ReadType<std::uint16_t>();
            this->Count50 = this->m_Reader.ReadType<std::uint16_t>();
            this->CountGeki = this->m_Reader.ReadType<std::uint16_t>();
            this->CountKatu = this->m_Reader.ReadType<std::uint16_t>();
            this->CountMiss = this->m_Reader.ReadType<std::uint16_t>();
            this->Score = this->m_Reader.ReadType<std::uint32_t>();
            this->MaxCombo = this->m_Reader.ReadType<std::uint16_t>();
            this->Perfect = this->m_Reader.ReadType<std::uint8_t>();
            this->Mods = this->m_Reader.ReadType<std::uint32_t>();
            this->LifeBar = this->m_Reader.ReadString();
            this->Timestamp = this->m_Reader.ReadType<std::uint64_t>();
            this->ReplayLength = this->m_Reader.ReadType<std::uint32_t>();

            if(this->ReplayLength > 0)
            {   
                std::uint8_t* ReplayBytes = new std::uint8_t[ReplayLength];
                this->m_Reader.GetStream().read(reinterpret_cast<char*>(ReplayBytes), ReplayLength);
                
                std::vector<std::uint8_t> DecompressedBytes = {};
                plz::PocketLzma LZMA;
                LZMA.decompress(ReplayBytes, ReplayLength, DecompressedBytes);
                DecompressedBytes[ReplayLength - 1] = '\0';
            
                std::string ReplayString = reinterpret_cast<char*>(DecompressedBytes.data());
                std::vector<std::string> ActionStrings = Utilities::Split(ReplayString, ',');
                std::int64_t TotalSongTime = 0;

                for(const std::string& Action : ActionStrings)
                {
                    std::vector<std::string> SplitAction = Utilities::Split(Action, '|');
                    if(SplitAction.size() == 4)
                    {
                        ReplayAction Action;
                        Action.Offset = std::stol(SplitAction[0]);
                        Action.X = std::stof(SplitAction[1]);
                        Action.Y = std::stof(SplitAction[2]);
                        Action.Keys = std::stoi(SplitAction[3]);
                        if(Action.Offset == -12345 || Action.Offset < 0 || (!Action.Offset && !TotalSongTime))
                            continue;
                        TotalSongTime += Action.Offset;
                        Action.Offset = TotalSongTime;
                        this->Actions.push_back(Action);
                    }
                }

                this->OnlineScoreID = this->m_Reader.ReadType<std::uint64_t>();
                delete[] ReplayBytes;
            }
        }
    private:
        void Reset() {}
    public:
        std::uint8_t ReplayMode;
        std::uint32_t Version;
        std::string BeatmapHash;
        std::string PlayerName;
        std::string ReplayHash;
        std::uint16_t Count300;
        std::uint16_t Count100;
        std::uint16_t Count50;
        std::uint16_t CountGeki;
        std::uint16_t CountKatu;
        std::uint16_t CountMiss;
        std::uint32_t Score;
        std::uint16_t MaxCombo;
        std::uint8_t Perfect;
        std::uint32_t Mods;
        std::string LifeBar;
        std::uint64_t Timestamp;
        std::uint32_t ReplayLength;
        std::vector<ReplayAction> Actions;
        std::uint64_t OnlineScoreID;
    private:
        Reader m_Reader;
    };
}