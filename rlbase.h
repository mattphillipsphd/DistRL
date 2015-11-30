#ifndef RLBASE_H
#define RLBASE_H

#include "globals.h"
#include "state.h"
#include "value.h"

//Critical to distinguish *global* state indices from chunk-local state indices
class RLBase
{
    public:
        RLBase(double gamma) : _gamma(gamma) {}
        ~RLBase() {}

        void Evaluate(const std::vector<const State*>& states, int nA, Value* vp)
        {
            const int nS = states.size();
            double* V = vp->V;
            int* policy = vp->policy;
//            for (int s=0; s<nS; ++s)
//                std::cout << states[s]->Id() << ", " << V[s] << std::endl;

            int ct=0;
            bool is_policy_stable = false;
            double* Vprev = new double[nS];
            while (!is_policy_stable && ct++<MAX_ITER)
            {
                //Policy evaluation
                memcpy(Vprev, V, sizeof(double)*nS);
                for (int s=0; s<nS; ++s)
                {
                    int a = policy[s]; //Deterministic policies
                    const State* state = states.at(s);
                    double acc = 0.0;
                    std::unordered_set<int> statep = state->NextStates();
                    for (auto it : statep)
                    {
                        auto iter = std::find_if(states.cbegin(), states.cend(), [it](const State* state)
                        {
                            return it == state->Id();
                        });
                        const int sp = iter - states.cbegin();
                        acc += state->Prob(it, a) * (state->Rew(it, a) + _gamma*Vprev[sp]);
                    }
                    if (!statep.empty()) //We let absorbing states hold onto their values
                        V[s] = acc;
                }

                //Policy improvement
                is_policy_stable = true;
                for (int s=0; s<nS; ++s)
                {
                    const State* state = states.at(s);
                    int b = policy[s];
                    std::vector<double> A_sp;
                    for (int a=0; a<nA; ++a)
                    {
                        double acc = 0.0;
                        std::unordered_set<int> statep = state->NextStates();
                        for (auto it : statep)
                        {
                            auto iter = std::find_if(states.cbegin(), states.cend(), [it](const State* state)
                            {
                                return it == state->Id();
                            });
                            const int sp = iter - states.cbegin();
                            acc += state->Prob(it, a) * (state->Rew(it, a) + _gamma*V[sp]);
                        }
                        A_sp.push_back(acc);
                    }
                    auto it = std::max_element(A_sp.cbegin(), A_sp.cend());
                    int idx = it - A_sp.cbegin();
                    policy[s] = idx;
                    if (b != idx)
                    {
                        is_policy_stable = false;
                        break;
                    }
                }
            }
//            for (int s=0; s<nS; ++s)
//                std::cout << states[s]->Id() << ", " << V[s] << std::endl;

            //Now associate the absorbing states with gamma times the mean of the
            //value of all states that project to them
            for (int s=0; s<nS; ++s)
            {
                const State* state = states.at(s);
                if ( state->NextStates().empty() )
                {
                    const std::vector<int>& bs = state->BackStates();
                    double acc = 0.0;
                    int ct = 0;
                    for (auto it : bs)
                    {
                        auto iter = std::find_if(states.cbegin(), states.cend(), [it](const State* state)
                        {
                            return it == state->Id();
                        });
                        if (iter == states.cend()) continue; //Can happen for closure states
                        const int sp = iter - states.cbegin();
                        const State* sprev = states.at(sp);
                        for (int a=0; a<nA; ++a)
                            if (sprev->Prob(state->Id(), a) > 0)
                            {
                                acc += _gamma * V[sp];
                                ++ct;
                            }
                    }
                    V[s] = acc/ct;
               }
            }

        }

    private:
        const int MAX_ITER = 1000;

        const double _gamma;
};

#endif // RLBASE_H
