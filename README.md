# osu!parser [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

### A C++ library to parse osu! filetypes

# Installation
```
git clone --recursive https://github.com/JustCabbage/osu-parser.git
```
## Project Setup
```CMake
target_include_directories(ProjectName PUBLIC osu-parser/include)
```
# Examples:

## Database Parsing
```c++
#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = std::string(getenv("localappdata")) + "\\osu!\\";

    OsuParser::Database ParsedDatabase(GamePath + "osu!.db");

    for(const auto& Beatmap : ParsedDatabase.Beatmaps)
    {
        std::cout << Beatmap.BeatmapHash << "\n";
    }
}
```

## Beatmap Parsing
```c++
#include <osu!parser/Parser.hpp>

int main()
{
    const std::string SongsPath = std::string(getenv("localappdata")) + "\\osu!\\Songs\\RandomBeatmap\\Beatmap.osu";
    
    OsuParser::Beatmap ParsedBeatmap(SongsPath);

    std::cout << "Audio Filename - " << ParsedBeatmap.General.AudioFilename << "\n";
    std::cout << "Total Hit Objects - " << ParsedBeatmap.HitObjects.size() << "\n";
    std::cout << "Total Timing Points - " << ParsedBeatmap.TimingPoints.size() << "\n";
}
```

## Replay Parsing
```c++
#include <osu!parser/Parser.hpp>

int main()
{
    const std::string ReplayPath = std::string(std::getenv("localappdata")) + "\\osu!\\Replays\\MyReplay.osr";
    
    OsuParser::Replay ParsedReplay(ReplayPath);

    std::cout << "Parsed " << ParsedReplay.Actions.size() << " Replay Actions\n";
    std::cout << "Replay Score - " << ParsedReplay.Score << "\n";
}
```

# Credits
- [osu!wiki](https://github.com/ppy/osu/wiki/)
- [pocketlzma](https://github.com/SSBMTonberry/pocketlzma)