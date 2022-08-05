#pragma once
#include <string>
#include <map>

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
        void InsertAttribute(const std::string& Key, const std::string& Value)
        {
            this->m_SectionMap[Key] = Value;
        }
    private:
        std::map<std::string,std::string> m_SectionMap = {};
    };
}