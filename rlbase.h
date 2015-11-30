#ifndef RLBASE_H
#define RLBASE_H

#include "globals.h"
#include "state.h"
#include "value.h"

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
            memset(V, 0, sizeof(double)*nS);

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
                    for (int sp=0; sp<nS; ++sp)
                        acc += state->Prob(sp, a) * (state->Rew(sp, a) + _gamma*Vprev[sp]);
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
                        for (int sp=0; sp<nS; ++sp)
                            acc += state->Prob(sp, a) * (state->Rew(sp, a) + _gamma*V[sp]);
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
        }

    private:
        const int MAX_ITER = 1000;

        const double _gamma;
};

#endif // RLBASE_H
