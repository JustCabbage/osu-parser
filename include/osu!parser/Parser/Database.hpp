#pragma once
#define LEGACY 20140609
#include <string>
#include <vector>
#include "Reader/Reader.hpp"
#include "Structures/BeatmapEntry.hpp"
#include "Structures/Enums.hpp"

namespace Parser
{
    class Database
    {
    public:
        Database(const std::string& DatabasePath) : m_Reader(DatabasePath) 
        { 
            this->OsuVersion = this->m_Reader.ReadType<std::int32_t>();
            this->FolderCount = this->m_Reader.ReadType<std::int32_t>();
            this->AccountUnlocked = this->m_Reader.ReadType<bool>();
            this->DateTime = this->m_Reader.ReadType<std::int64_t>();
            this->PlayerName = this->m_Reader.ReadString();
            this->TotalBeatmaps = this->m_Reader.ReadType<std::int32_t>();
            for(int i = 0;  i < this->TotalBeatmaps; i++)
            {
                BeatmapEntry Entry;
                Entry.Artist = this->m_Reader.ReadString();
                Entry.ArtistUnicode = this->m_Reader.ReadString();
                Entry.Title = this->m_Reader.ReadString();
                Entry.TitleUnicode = this->m_Reader.ReadString();
                Entry.Creator = this->m_Reader.ReadString();
                Entry.Difficulty = this->m_Reader.ReadString();
                Entry.SongPath = this->m_Reader.ReadString();
                Entry.BeatmapHash = this->m_Reader.ReadString();
                Entry.BeatmapPath = this->m_Reader.ReadString();
                Entry.RankedStatus = this->m_Reader.ReadType<std::uint8_t>();
                Entry.CircleCount = this->m_Reader.ReadType<std::int16_t>();
                Entry.SliderCount = this->m_Reader.ReadType<std::int16_t>();
                Entry.SpinnerCount = this->m_Reader.ReadType<std::int16_t>();
                Entry.LastModificationTime = this->m_Reader.ReadType<std::int64_t>();
                Entry.ApproachRate = this->m_Reader.ReadType<std::float_t>();
                Entry.CircleSize = this->m_Reader.ReadType<std::float_t>();
                Entry.HealthDrainRate = this->m_Reader.ReadType<std::float_t>();
                Entry.OverallDifficulty = this->m_Reader.ReadType<std::float_t>(); 
                Entry.SliderVelocity = this->m_Reader.ReadType<std::double_t>();

                for(int j = 0; j < 4; j++)
                {
                    // Add Dictionary Parsing
                    std::int32_t Count = this->m_Reader.ReadType<std::int32_t>();
                    for(int k = 0; k < Count; k++)
                    {
                        this->m_Reader.Seek(14);
                    }
                }

                Entry.DrainTime = this->m_Reader.ReadType<std::int32_t>();
                Entry.TotalTime = this->m_Reader.ReadType<std::int32_t>();
                Entry.HoverPreviewTime = this->m_Reader.ReadType<std::int32_t>();
                
                std::int32_t Count = this->m_Reader.ReadType<std::int32_t>();
                for(int j = 0; j < Count; j++)
                {
                    // Add Timing Point Parsing
                    this->m_Reader.Seek(17);
                }

                Entry.DifficultyID = this->m_Reader.ReadType<std::int32_t>();
                Entry.BeatmapID = this->m_Reader.ReadType<std::int32_t>();
                Entry.ThreadID = this->m_Reader.ReadType<std::int32_t>();
                Entry.GradeStandard = this->m_Reader.ReadType<std::uint8_t>();
                Entry.GradeTaiko = this->m_Reader.ReadType<std::uint8_t>();
                Entry.GradeCTB = this->m_Reader.ReadType<std::uint8_t>();
                Entry.GradeMania = this->m_Reader.ReadType<std::uint8_t>();
                Entry.BeatmapOffset = this->m_Reader.ReadType<std::int16_t>();
                Entry.StackLeniency = this->m_Reader.ReadType<std::float_t>();
                Entry.GameplayMode = this->m_Reader.ReadType<std::uint8_t>();
                Entry.SongSource = this->m_Reader.ReadString();
                Entry.SongTags = this->m_Reader.ReadString();
                Entry.OnlineOffset = this->m_Reader.ReadType<std::int16_t>();
                Entry.TitleFont = this->m_Reader.ReadString();
                Entry.IsUnplayed = this->m_Reader.ReadType<bool>();
                Entry.LastPlayTime = this->m_Reader.ReadType<std::int64_t>();
                Entry.IsOSZ2 = this->m_Reader.ReadType<bool>();
                Entry.FolderName = this->m_Reader.ReadString();
                Entry.LastChecked = this->m_Reader.ReadType<std::int64_t>();
                Entry.IgnoreSound = this->m_Reader.ReadType<bool>();
                Entry.IgnoreSkin = this->m_Reader.ReadType<bool>();
                Entry.DisableStoryboard = this->m_Reader.ReadType<bool>();
                Entry.DisableVideo = this->m_Reader.ReadType<bool>();
                Entry.VisualOverride = this->m_Reader.ReadType<bool>();
                std::int32_t Unknown = this->m_Reader.ReadType<std::int32_t>();
                Entry.ManiaScrollSpeed = this->m_Reader.ReadType<std::uint8_t>();
                this->Beatmaps.push_back(Entry);
            }
            this->Permissions = static_cast<Permission>(this->m_Reader.ReadType<std::int32_t>());
        }
    public:
        std::int32_t OsuVersion = 0;
        std::int32_t FolderCount = 0;
        bool AccountUnlocked = false;
        std::int64_t DateTime = 0;
        std::string PlayerName = "N/A";
        std::int32_t TotalBeatmaps = 0;
        std::vector<BeatmapEntry> Beatmaps = {};
        Permission Permissions = Permission::None;
    private:
        Reader m_Reader;
    };
}