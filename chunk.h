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
            delete _vp;
        }

        void AddState(const State* state)
        {
            _states.push_back(state);
        }
        void Evaluate()
        {
            assert(_rl && "Algorithm not initialized");
            _rl->Evaluate(_states, _nA, _vp);
        }

        //This goes through and finds terminal states to account for all connections
        //severed in the cutting process.  It does *not* add them, but returns them.
        std::vector<int> Closure()
        {
            std::vector<int> vs;
            std::unordered_set<int> sp;
            for (auto it : _states)
            {
                std::unordered_set<int> sp_i = it->NextStates();
                for (auto it : sp_i) sp.insert(it);
            }
            auto s_end = _states.cend();
            for (auto it : sp)
            {
                auto iter = std::find_if(_states.cbegin(), _states.cend(), [it](const State* st)
                {
                    return it==st->Id();
                });
                if (iter == s_end)
                    vs.push_back(it);
            }

            return vs;
        }

        //Initialize the Value object belonging to this state from the master Value object
        void Initialize(const Value& master)
        {
            if (_vp) delete _vp;
            const int nS_local = _states.size();
            _vp = new Value(nS_local);
            for (int s=0; s<nS_local; ++s)
            {
                const State* state = _states.at(s);
                _vp->V[s] = master.V[ state->Id() ];
                _vp->policy[s] = master.policy[ state->Id() ];
            }
        }

        //Update the master Value object on the basis of the local Value object
        void Update(Value& master) const
        {
            const int nS_local = _states.size();
            for (int s=0; s<nS_local; ++s)
            {
                const State* state = _states.at(s);
                if (!state->IsTerminal())
                {
                    master.V[ state->Id() ] = _vp->V[s];
                    master.policy[ state->Id() ] = _vp->policy[s];
                }
            }
        }

        void SetRL(RLBase* rl) { _rl = rl; }
        const Value* GetValue() const { return _vp; }
        const int* Policy() const { return _vp->policy; }
        const double* V() const { return _vp->V; }

    private:
        const int _nA; //Can also just extract this from the states
        std::vector<const State*> _states;
        RLBase* _rl;
        Value* _vp;
};

#endif // CHUNK_H
