#pragma once
#include <iostream>

namespace Parser
{
    enum class Permission : std::uint32_t
    {
        None = 0,
        Normal,
        Moderator,
        Supporter,
        Friend,
        Peppy,
        WorldCupStaff
    };
}