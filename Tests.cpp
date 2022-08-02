#include <osu!parser/Parser.hpp>

int main()
{
    Parser::Database ParsedDatabase(std::string(getenv("localappdata")) + "\\osu!\\osu!.db");
    
    for(const auto& Beatmap : ParsedDatabase.Beatmaps)
    {
        std::cout << Beatmap.BeatmapHash << "\n";
    }
}