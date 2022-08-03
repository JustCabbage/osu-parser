# osu!parser [![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)

### A C++ library to parse osu! filetypes

# Examples:

## Database Parsing
```c++
#include <osu!parser/Parser.hpp>

int main()
{
    const std::string GamePath = std::string(getenv("localappdata")) + "\\osu!\\";

    Parser::Database ParsedDatabase(GamePath + "osu!.db");

    for(const auto& Beatmap : ParsedDatabase.Beatmaps)
    {
        std::cout << Beatmap.BeatmapHash << "\n";
    }
}
```

# Credits
- [osu!wiki](https://github.com/ppy/osu/wiki/)