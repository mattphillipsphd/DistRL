#include "chunkmgr.h"



void ChunkMgr::Launch()
{
    for (auto it : _chunks)
    {
        std::thread t(it.Launch());
    }
}
