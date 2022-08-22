#pragma once
#include <string>
#include <vector>

namespace Parser::Utilities
{
    inline std::vector<std::string> Split(const std::string &Input, char Delimeter) 
    {
        std::vector<std::string> Output;
        std::stringstream Stream(Input);
        std::string CurrentLine = "";

        while (std::getline(Stream, CurrentLine, Delimeter))
        {
            Output.push_back(CurrentLine);
        }

        return Output;
    }

    inline std::string Trim(std::string Input)
    {
        return Input.erase(Input.find_last_not_of(' ')+1).erase(0, Input.find_first_not_of(' '));
    }
}