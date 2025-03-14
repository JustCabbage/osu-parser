#pragma once
#include <iostream>

struct TimingPointEntry
{
    std::double_t BPM;
    std::double_t Offset;
    bool NotInherited;
};