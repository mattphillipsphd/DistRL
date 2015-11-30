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

    double* V;
    int* policy;
    const int nS;
};

#endif // VALUE

