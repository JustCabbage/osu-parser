#pragma once
#include <map>
#include <osu!parser/Parser/Utilities.hpp>
#include <string>

namespace OsuParser::Beatmap::Sections
{
    class Section
    {
    public:
        virtual ~Section() = default;
        virtual void Parse(std::vector<std::string>& Lines) = 0;

    protected:
        std::string GetAttribute(const std::string& Key)
        {
            return !this->m_SectionMap[Key].empty() ? this->m_SectionMap[Key] : "";
            // if not found, should return empty string because may be "N/A" is known as a valid value
            // you can check by using std::string::empty()
        }

        void LoadAttributes(const std::vector<std::string>& Lines)
        {
            for (const std::string& Line : Lines)
            {
                if (Line.find(':') == std::string::npos)
                {
                    continue;
                }
                std::vector<std::string> SplitLine = Utilities::Split(Line, ':', true);
                for (auto &str: SplitLine) str = Utilities::Trim(str);

                if (SplitLine.size() > 1)
                {
                    this->InsertAttribute(SplitLine[0], Utilities::Trim(SplitLine[1]));
                }
            }
        }

        void InsertAttribute(const std::string& Key, const std::string& Value)
        {
            this->m_SectionMap[Key] = Value;
        }

    private:
        std::map<std::string, std::string> m_SectionMap = {};
    };
} // namespace Parser