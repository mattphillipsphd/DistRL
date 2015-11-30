#ifndef VALUE
#define VALUE

//This encodes a deterministic policy only.
struct Value
{
    Value(int nS) : nS(nS)
    {
        V = new double[nS];
        policy = new int[nS];
    }
    ~Value()
    {
        delete[] V;
        delete[] policy;
    }
    void Init0()
    {
        memset(V, 0, sizeof(double)*nS);
        memset(policy, 0, sizeof(int)*nS);
    }
    void Write(std::ostream& out = std::cout) const
    {
        for (int s=0; s<nS; ++s)
            out << "\t" << V[s];
        out << std::endl;
        for (int s=0; s<nS; ++s)
            out << "\t" << policy[s];
        out << std::endl;
    }

    double* V;
    int* policy;
    const int nS;
};

#endif // VALUE

