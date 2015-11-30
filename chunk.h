#ifndef CHUNK_H
#define CHUNK_H

#include "globals.h"
#include "rlbase.h"
#include "state.h"

class Chunk
{
    public:
        Chunk(int nA) : _nA(nA), _vp(nullptr) {}
        ~Chunk()
        {
            delete _rl;
            delete _vp;
        }

        void AddState(const State* state)
        {
            _states.push_back(state);
        }
        void Evaluate()
        {
            assert(_rl && "Algorithm not initialized");
            if (_vp) delete _vp;
            _vp = new Value( _states.size() );
            _rl->Evaluate(_states, _nA, _vp);
        }
        void SetRL(RLBase* rl) { _rl = rl; }
        const int* Policy() const { return _vp->policy; }
        const double* V() const { return _vp->V; }

    private:
        const int _nA; //Can also just extract this from the states
        std::vector<const State*> _states;
        RLBase* _rl;
        Value* _vp;
};

#endif // CHUNK_H
