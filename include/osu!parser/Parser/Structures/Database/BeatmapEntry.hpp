#pragma once
#include <string>
#include <vector>

#include "TimingPointEntry.hpp"

namespace OsuParser
{
    struct BeatmapEntry
    {
        std::uint32_t Size;
        std::string Artist;
        std::string ArtistUnicode;
        std::string Title;
        std::string TitleUnicode;
        std::string Creator;
        std::string Difficulty;
        std::string SongPath;
        std::string BeatmapHash;
        std::string BeatmapPath;
        std::uint8_t RankedStatus;
        std::uint16_t CircleCount;
        std::uint16_t SliderCount;
        std::uint16_t SpinnerCount;
        std::uint64_t LastModificationTime;
        std::float_t ApproachRate;
        std::float_t CircleSize;
        std::float_t HealthDrainRate;
        std::float_t OverallDifficulty;
        std::double_t SliderVelocity;
        std::int32_t DrainTime;
        std::int32_t TotalTime;
        std::int32_t HoverPreviewTime;
        std::int32_t DifficultyID;
        std::int32_t BeatmapID;
        std::int32_t ThreadID;
        std::uint8_t GradeStandard;
        std::uint8_t GradeTaiko;
        std::uint8_t GradeCTB;
        std::uint8_t GradeMania;
        std::int16_t BeatmapOffset;
        std::float_t StackLeniency;
        std::uint8_t GameplayMode;
        std::string SongSource;
        std::string SongTags;
        std::int16_t OnlineOffset;
        std::string TitleFont;
        bool IsUnplayed;
        std::int64_t LastPlayTime;
        bool IsOSZ2;
        std::string FolderName;
        std::int64_t LastChecked;
        bool IgnoreSound;
        bool IgnoreSkin;
        bool DisableStoryboard;
        bool DisableVideo;
        bool VisualOverride;
        std::uint8_t ManiaScrollSpeed;
        std::vector<TimingPointEntry> TimingPoints;
    };
} // namespace Parser