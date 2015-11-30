#include "chunk.h"
#include "chunkmgr.h"
#include "state.h"
#include "transprob.h"

int main()
{
    const int nA = 2, //0=Left or 1=right
            nS = 13; //Start with 1d Markov chain
    const int num_chunks = 2;

    TransProb P(nS, nA);

    for (int s=1; s<nS-1; ++s)
    {
        P.Val(s, s-1, 0) = 1;
        P.Val(s, s+1, 1) = 1;
    }

    //One reward state, another 0 state, everything else negative
    TransProb R(nS, nA, -1.0/(double)(nS-3));
    R.Val(1, 0, 0) = 0;
    R.Val(nS-2, nS-1, 1) = 0.6;

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

        states.push_back( new State(s, vs, Pfmap, Rfmap) );
    }

    //Put the states into chunks
    std::vector<Chunk*> chunks;
    for (int c=0; c<num_chunks; ++c)
        chunks.push_back( new Chunk(nA) );

    for (int i=0; i<nS; ++i)
        chunks.at( (num_chunks*i)/nS )->AddState( states.at(i) );

    Value master(nS);
    master.Init0();
    for (int c=0; c<num_chunks; ++c)
    {
        Chunk* chunk = chunks[c];

        //Add nodes to get closure
        std::vector<int> closure = chunk->Closure();
        for (auto it : closure)
            chunk->AddState( new State(it, P.FindS(it)) );

        //Initialize with appropriate algorithm
        const double gamma = 1;
        RLBase* rl = new RLBase(gamma);
        chunk->SetRL(rl);
    }

    for (int i=0; i<10; ++i)
    {
        for (int c=0; c<num_chunks; ++c)
        {
            Chunk* chunk = chunks[c];

            //Initialize with the current master list
            chunk->Initialize(master);

            //Evaluate the chunk
            chunk->Evaluate();
//            std::cout << "\nChunk " << c << ": " << std::endl;
//            chunk->GetValue()->Write();
            chunk->Update(master);

//            std::cout << "\nChunk " << c << ": " << std::endl;
//            master.Write();

        }
    }

    std::cout << "Final valuation and policy:" << std::endl;
    master.Write();

    //Gather results
/*    for (int c=0; c<num_chunks; ++c)
    {
        Chunk* chunk = chunks[c];
        std::cout << "Chunk " << c << ": " << std::endl;
        chunk->GetValue()->Write();
    }
*/
//    delete rl;
    for (auto it : states) delete it;

    return 0;
}

