#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = "D:\\PROGRAM\\osu!"; // Your osu! path here
    const std::string SongsPath = GamePath + "\\Songs\\";
    const std::string ReplaysPath = GamePath + "\\Replays\\";

    // Database
    // const Parser::Database ParsedDatabase(GamePath + "osu!.db");
    // std::cout << "Parsed " << ParsedDatabase.Beatmaps.size() << " Beatmaps\n";

    // Beatmap
    // const Parser::BeatmapEntry Beatmap = ParsedDatabase.Beatmaps[0];
    // const Parser::Beatmap ParsedBeatmap(SongsPath + Beatmap.FolderName + "\\" + Beatmap.BeatmapPath);
    const std::string beatmap_file = R"(D:\PROGRAM\osu!\Songs\1511778 Camellia - Body F10ating in the Zero Gravity Space\Camellia - Body F10ating in the Zero Gravity Space (Orange_) [Nonsubmersible].osu)";
    const OsuParser::Beatmap::Beatmap ParsedBeatmap(beatmap_file);
    std::cout << "Overall Difficulty - " << ParsedBeatmap.Difficulty.OverallDifficulty << "\n";
    std::cout << "Audio Filename - " << ParsedBeatmap.General.AudioFilename << "\n";
    std::cout << "Artist - " << ParsedBeatmap.Metadata.Artist << "\n";
    std::cout << "Approach Rate - " << ParsedBeatmap.Difficulty.ApproachRate << "\n";
    std::cout << "Grid Size - " << ParsedBeatmap.Editor.GridSize << "\n";
    std::cout << "Total Hit Objects - " << ParsedBeatmap.HitObjects.data.size() << "\n";
    std::cout << "Total Timing Points - " << ParsedBeatmap.TimingPoints.data.size() << "\n";
    std::cout << "Total Events - " << ParsedBeatmap.Events.objects.size() << "\n";

    // Replay
    // const Parser::Replay ParsedReplay(ReplaysPath + "MyReplay.osr");
    // std::cout << "Parsed " << ParsedReplay.Actions.size() << " Replay Actions\n";
    // std::cout << "Replay Score - " << ParsedReplay.Score << "\n";
    std::cout << "--- The End ---\n";

    std::ofstream out("D:/test.osu");
    out << ParsedBeatmap.Events;
    out.close();

    // for (auto& data : ParsedBeatmap.Events.objects)
    // {
    //     if (auto it = std::dynamic_pointer_cast<OsuParser::Beatmap::Objects::Event::Objects::SpriteObject>(data))
    //     {
    //         if (it->filepath == R"(sb\TitleBlock.jpg)")
    //         {
    //             int a = 0;
    //         }
    //     }
    // }
    // Events tested successfully
}