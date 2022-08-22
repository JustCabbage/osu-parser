#pragma once
#include <string>
#include <map>
#include "../../../Utilities.hpp"

namespace Parser
{
    class Section
    {
    public:
        virtual void Parse(std::vector<std::string>& Lines) = 0;
    protected:
        std::string GetAttribute(const std::string& Key)
        {
            return this->m_SectionMap[Key].size() ? this->m_SectionMap[Key] : "N/A";
        }

        void LoadAttributes(const std::vector<std::string>& Lines)
        {
            for(const std::string& Line : Lines)
            {
                if(Line.find(":") == std::string::npos)
                {
                    continue;
                }
                std::vector<std::string> SplitLine = Utilities::Split(Line, ':');
                if(SplitLine.size() > 1)
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
        std::map<std::string,std::string> m_SectionMap = {};
    };
}