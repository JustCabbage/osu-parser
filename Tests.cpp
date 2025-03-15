#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = "D://Games//osu!//"; // Your osu! path here
    const std::string SongsPath = GamePath + "\\Songs\\";
    const std::string ReplaysPath = GamePath + "\\Replays\\";

    const Parser::Database ParsedDatabase(GamePath + "osu!.db");

    std::cout << "Parsed " << ParsedDatabase.Beatmaps.size() << " Beatmaps\n";

    const Parser::BeatmapEntry Beatmap = ParsedDatabase.Beatmaps[0];

    const Parser::Beatmap ParsedBeatmap(SongsPath + Beatmap.FolderName + "\\" + Beatmap.BeatmapPath);

    std::cout << "Overall Difficulty - " << ParsedBeatmap.Difficulty.OverallDifficulty << "\n";
    std::cout << "Audio Filename - " << ParsedBeatmap.General.AudioFilename << "\n";
    std::cout << "Artist - " << ParsedBeatmap.Metadata.Artist << "\n";
    std::cout << "Approach Rate - " << ParsedBeatmap.Difficulty.ApproachRate << "\n";
    std::cout << "Grid Size - " << ParsedBeatmap.Editor.GridSize << "\n";
    std::cout << "Total Hit Objects - " << ParsedBeatmap.HitObjects.size() << "\n";
    std::cout << "Total Timing Points - " << ParsedBeatmap.TimingPoints.size() << "\n";

    const Parser::Replay ParsedReplay(ReplaysPath + "MyReplay.osr");

    std::cout << "Parsed " << ParsedReplay.Actions.size() << " Replay Actions\n";

    std::cout << "Replay Score - " << ParsedReplay.Score << "\n";
}