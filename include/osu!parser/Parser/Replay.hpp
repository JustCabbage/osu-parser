#pragma once
#include <string>
#include <fstream>

namespace Parser
{
    class Replay
    {
    public:
        Replay(const std::string& ReplayPath) : m_CurrentStream(ReplayPath)
        {
            this->Reset();
            if(!m_CurrentStream.good())
            {
                return;
            }
        }
    private:
        void Reset() {}
    public:
        // Publically Accessible Fields
    private:
        std::ifstream m_CurrentStream;
    };
}