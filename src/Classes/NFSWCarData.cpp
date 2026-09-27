#include "NFSWCarData.h"

namespace
{
struct Row { tID id; tValueF v[12]; };
static const Row kRows[] =
{
#include "NFSWCarDataRows1.inc"
#include "NFSWCarDataRows2.inc"
#include "NFSWCarDataRows3.inc"
#include "NFSWCarDataRows4.inc"
};
}

bool NFSWCarData::Lookup(const tID& id, tValueF topspeed[4], tValueF acceleration[4], tValueF handling[4])
{
    const int count = sizeof(kRows) / sizeof(kRows[0]);
    for(int i = 0; i < count; ++i)
    {
        if(kRows[i].id != id) continue;
        for(int n = 0; n < 4; ++n)
        {
            topspeed[n] = kRows[i].v[n];
            acceleration[n] = kRows[i].v[4 + n];
            handling[n] = kRows[i].v[8 + n];
        }
        return true;
    }
    return false;
}
