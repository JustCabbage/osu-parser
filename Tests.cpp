#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = std::string(getenv("localappdata")) + "\\osu!\\";
    const std::string SongsPath = std::string(getenv("localappdata")) + "\\osu!\\Songs\\";

    const Parser::Database ParsedDatabase(GamePath + "osu!.db");

    for(const auto& Beatmap : ParsedDatabase.Beatmaps)
    {
        std::cout << Beatmap.BeatmapHash << "\n";
    }

    const Parser::BeatmapEntry Beatmap = ParsedDatabase.Beatmaps[0];

    const Parser::Beatmap ParsedBeatmap(SongsPath + Beatmap.FolderName + "\\" + Beatmap.BeatmapPath);

    std::cout << ParsedBeatmap.General.AudioFilename << "\n";
}