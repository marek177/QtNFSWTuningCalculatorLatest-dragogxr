#ifndef __NFSWCARDATA_H__
#define __NFSWCARDATA_H__

#include "TypeDefinitions.h"

namespace NFSWCarData
{
    bool Lookup(const tID& id, tValueF topspeed[4], tValueF acceleration[4], tValueF handling[4]);
}

#endif /* __NFSWCARDATA_H__ */
