#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace OsuParser::Utilities
{
    inline std::vector<std::string> Split(const std::string& Input, char Delimeter, bool onlyTwoPart = false)
    {
        std::vector<std::string> Output;
        std::stringstream Stream(Input);
        std::string CurrentLine;

        /**
		 *  Why onlyTwoPart?
		 *
		 *  Because beatmap name can be "Operation: Zenithfall" - https://osu.ppy.sh/beatmapsets/2287992#osu/4881796
		 *  (includes ':' in name).
         */
        if (onlyTwoPart)
        {
            std::getline(Stream, CurrentLine, Delimeter);
            Output.push_back(CurrentLine);
            std::getline(Stream, CurrentLine);
            Output.push_back(CurrentLine);
        }
        else
        {
            while (std::getline(Stream, CurrentLine, Delimeter))
            {
                Output.push_back(CurrentLine);
            }
        }

        return Output;
    }

    inline std::string Trim(std::string Input, const bool onlyRight = false, const char& space = ' ')
    {
        if (onlyRight)
            Input.erase(Input.find_last_not_of(space) + 1);
        else
            Input.erase(Input.find_last_not_of(space) + 1).erase(0, Input.find_first_not_of(space));

        return Input;
    }
}
