#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = std::string(std::getenv("localappdata")) + "\\osu!\\";
    const std::string SongsPath = std::string(std::getenv("localappdata")) + "\\osu!\\Songs\\";

    const Parser::Database ParsedDatabase(GamePath + "osu!.db");

    for(const auto& Beatmap : ParsedDatabase.Beatmaps)
    {
        std::cout << Beatmap.BeatmapHash << "\n";
    }

    const Parser::BeatmapEntry Beatmap = ParsedDatabase.Beatmaps[0];

    const Parser::Beatmap ParsedBeatmap(SongsPath + Beatmap.FolderName + "\\" + Beatmap.BeatmapPath);

    std::cout << "Audio Filename - " << ParsedBeatmap.General.AudioFilename << "\n";
    std::cout << "Artist - " << ParsedBeatmap.Metadata.Artist << "\n";
    std::cout << "Approach Rate - " << ParsedBeatmap.Difficulty.ApproachRate << "\n";
    std::cout << "Grid Size - " << ParsedBeatmap.Editor.GridSize << "\n";
    std::cout << "Total Hit Objects - " << ParsedBeatmap.HitObjects.size() << "\n";
    std::cout << "Total Timing Points - " << ParsedBeatmap.TimingPoints.size() << "\n";
}