#pragma once
#include <string>
#include <unordered_map>
#include "Section.hpp"

namespace OsuParser::Beatmap::Sections::Variable
{
    struct VariableSection final : public Section
    {
        std::unordered_map<std::string, std::string> Variables;

        void Parse(std::vector<std::string>& Lines) override
        {
            this->LoadAttributes(Lines);

            for (const auto& line : Lines)
            {
                if (const auto parts = Utilities::Split(line, '=', true);
                    parts.size() >= 2)
                    Variables[Utilities::Trim(parts[0])] = Utilities::Trim(parts[1]);
            }
        }

        [[nodiscard]] std::string GetVariable(const std::string& name) const
        {
            if (const auto it = Variables.find(name);
                it != Variables.end())
            {
                return it->second;
            }
            return {};
        }

        void ProvideVariable(std::string& line) const
        {
            for (auto begin = line.begin(); begin != line.end(); ++begin)
            {
                while (begin != line.end() && *begin == '$')
                {
                    auto end = begin + 1;
                    while (end != line.end() && *end != ',') ++end;

                    if (const auto it = Variables.find(std::string(begin, end));
                        it == Variables.end())
                    {
                        begin = end;
                    }
                    else line.replace(begin, end, it->second);
                }
            }
        }
    };
}
