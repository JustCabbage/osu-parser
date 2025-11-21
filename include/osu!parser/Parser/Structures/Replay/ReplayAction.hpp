#pragma once
#include <iostream>

namespace OsuParser
{
    struct ReplayAction
    {
        std::int64_t Offset;
        std::float_t X, Y;
        std::int32_t Keys;
    };
}