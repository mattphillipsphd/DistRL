#ifndef CHUNKMGR_H
#define CHUNKMGR_H

#include "chunk.h"
#include "value.h"

class ChunkMgr
{
    public:
        ChunkMgr(Value* master) : _master(master) {}
        ~ChunkMgr() {}

        void AddChunk(Chunk* chunk) { _chunks.push_back(chunk); }
        void Launch();

    private:
        std::vector<Chunk*> _chunks;
        Value* _master;
};

#endif // CHUNKMGR_H
