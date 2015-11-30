#include "chunk.h"
#include "state.h"
#include "transprob.h"

int main()
{
    const int nA = 2, //0=Left or 1=right
            nS = 7; //Start with 1d Markov chain

    TransProb P(nS, nA);

    for (int s=1; s<nS-1; ++s)
    {
        P.Val(s, s-1, 0) = 1;
        P.Val(s, s+1, 1) = 1;
    }

    //One reward state, another 0 state, everything else negative
    TransProb R(nS, nA, -1.0/(double)(nS-3));
    R.Val(1, 0, 0) = 0;
    R.Val(nS-2, nS-1, 1) = 1;

    //Create all the states
    std::vector<State*> states;
    for (int s=0; s<nS; ++s)
    {
        std::vector<int> vs = P.FindS(s);

        Fmap Pfmap, Rfmap;
        for (int sp=0; sp<nS; ++sp)
            for (int a=0; a<nA; ++a)
                if (P.ValM(s,sp,a) > 0.0)
                {
                    Pfmap[a][sp] = P.Val(s,sp,a);
                    Rfmap[a][sp] = R.Val(s,sp,a);
                }

        states.push_back( new State(vs, Pfmap, Rfmap) );
    }

    //Put the states into chunks
    Chunk chunk(nA);
    for (auto it : states)
        chunk.AddState(it);

    //Initialize with appropriate algorithm
    const double gamma = 1;
    RLBase* rl = new RLBase(gamma);
    chunk.SetRL(rl);

    //Evaluate the chunk
    chunk.Evaluate();

    //Gather results
    const double* V = chunk.V();
    for (int s=0; s<nS; ++s)
        std::cout << V[s] << ", ";
    std::cout << std::endl;
    const int* policy = chunk.Policy();
    for (int s=0; s<nS; ++s)
        std::cout << policy[s] << ", ";
    std::cout << std::endl;

    delete rl;
    for (auto it : states) delete it;

    return 0;
}

