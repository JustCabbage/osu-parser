#pragma once
#define LEGACY 20140609
#include <string>
#include <vector>

#include "Reader/Reader.hpp"
#include "Structures/Database/BeatmapEntry.hpp"
#include "Structures/Database/Enums.hpp"

namespace OsuParser
{
    class Database
    {
    public:
        Database(const std::string& DatabasePath)
        {
            this->Reset();
            if (!m_Reader.SetStream(DatabasePath))
            {
                return;
            }
            this->OsuVersion = this->m_Reader.ReadType<std::int32_t>();
            this->FolderCount = this->m_Reader.ReadType<std::int32_t>();
            this->AccountUnlocked = this->m_Reader.ReadType<bool>();
            this->DateTime = this->m_Reader.ReadType<std::int64_t>();
            this->PlayerName = this->m_Reader.ReadString();
            this->TotalBeatmaps = this->m_Reader.ReadType<std::int32_t>();
            for (std::int32_t i = 0; i < this->TotalBeatmaps; i++)
            {
                BeatmapEntry Entry;
                if (this->OsuVersion < 20191106)
                    Entry.Size = this->m_Reader.ReadType<std::int32_t>();

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
                if (this->OsuVersion >= LEGACY)
                {
                    Entry.ApproachRate = this->m_Reader.ReadType<std::float_t>();
                    Entry.CircleSize = this->m_Reader.ReadType<std::float_t>();
                    Entry.HealthDrainRate = this->m_Reader.ReadType<std::float_t>();
                    Entry.OverallDifficulty = this->m_Reader.ReadType<std::float_t>();
                }
                else
                {
                    Entry.ApproachRate = this->m_Reader.ReadType<std::int8_t>();
                    Entry.CircleSize = this->m_Reader.ReadType<std::int8_t>();
                    Entry.HealthDrainRate = this->m_Reader.ReadType<std::int8_t>();
                    Entry.OverallDifficulty = this->m_Reader.ReadType<std::int8_t>();
                }
                Entry.SliderVelocity = this->m_Reader.ReadType<std::double_t>();

                if (this->OsuVersion >= LEGACY)
                {
                    for (std::int32_t j = 0; j < 4; j++)
                    {
                        // Add Dictionary Parsing
                        std::int32_t Count = this->m_Reader.ReadType<std::int32_t>();
                        for (std::int32_t k = 0; k < Count; k++)
                        {
                            this->m_Reader.Seek(this->OsuVersion > 2025107 ? 10 : 14);
                        }
                    }
                }

                Entry.DrainTime = this->m_Reader.ReadType<std::int32_t>();
                Entry.TotalTime = this->m_Reader.ReadType<std::int32_t>();
                Entry.HoverPreviewTime = this->m_Reader.ReadType<std::int32_t>();

                std::int32_t TimingPointCount = this->m_Reader.ReadType<std::int32_t>();
                for (std::int32_t j = 0; j < TimingPointCount; j++)
                {
                    TimingPointEntry TimingPoint;
                    TimingPoint.BPM = this->m_Reader.ReadType<std::double_t>();
                    TimingPoint.Offset = this->m_Reader.ReadType<std::double_t>();
                    TimingPoint.NotInherited = this->m_Reader.ReadType<bool>();
                    Entry.TimingPoints.push_back(TimingPoint);
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
                if (this->OsuVersion < LEGACY)
                {
                    std::int16_t UnknownShort = this->m_Reader.ReadType<std::int16_t>();
                }
                std::int32_t Unknown = this->m_Reader.ReadType<std::int32_t>();
                Entry.ManiaScrollSpeed = this->m_Reader.ReadType<std::uint8_t>();
                this->Beatmaps.push_back(Entry);
            }
            this->Permissions = static_cast<Permission>(this->m_Reader.ReadType<std::int32_t>());
        }
        ~Database()
        {
            this->Reset();
        }

    private:
        void Reset()
        {
            this->OsuVersion = 0;
            this->FolderCount = 0;
            this->AccountUnlocked = false;
            this->DateTime = 0;
            this->PlayerName = "N/A";
            this->TotalBeatmaps = 0;
            this->Beatmaps.clear();
            Permission Permissions = Permission::None;
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
} // namespace Parser