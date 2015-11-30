#ifndef TRANSPROB
#define TRANSPROB

#include "globals.h"

class TransProb
{
    public:
        TransProb(int nS, int nA, double defval = 0.0)
            : _d(new double[nS*nS*nA]), _nA(nA), _nS(nS)
        {
            if (defval == 0.0)
                memset(_d, 0, sizeof(double)*_nS*_nS*_nA);
            else
                for (int i=0; i<_nS*_nS*_nA; ++i)
                    _d[i] = defval;
        }
        ~TransProb()
        {
            delete[] _d;
        }
        void Write(std::ostream& out = std::cout)
        {
            for (int a=0; a<_nA; ++a)
            {
                out << "a = " << a << ":" << std::endl;
                for (int s=0; s<_nS; ++s)
                {
                    for (int sp=0; sp<_nS; ++sp)
                        out << "\t" << Val(s, sp, a);
                    out << std::endl;
                }
                out << std::endl;
            }
        }

        //Find all s which, for a given a taken from s, lead to sp with non-zero probability
        inline std::vector<int> FindS(int sp) const
        {
            std::vector<int> vs;
            vs.reserve(_nS);
            for (int a=0; a<_nA; ++a)
                for (int s=0; s<_nS; ++s)
                    if ( Val(s,sp,a) > MIN_PROB ) vs.push_back(s);
            return vs;
        }
        inline std::vector<int> FindS(int sp, int a) const
        {
            std::vector<int> vs;
            vs.reserve(_nS);
            for (int s=0; s<_nS; ++s)
                if ( Val(s,sp,a) > MIN_PROB ) vs.push_back(s);
            return vs;
        }

        //Find all sp which to which the agent can transition from s given action a
        inline std::vector<int> FindSp(int s) const
        {
            std::vector<int> vsp;
            vsp.reserve(_nS);
            for (int a=0; a<_nA; ++a)
                for (int sp=0; sp<_nS; ++sp)
                    if ( Val(s,sp,a) > MIN_PROB ) vsp.push_back(sp);
            return vsp;
        }
        inline std::vector<int> FindSp(int s, int a) const
        {
            std::vector<int> vsp;
            vsp.reserve(_nS);
            for (int sp=0; sp<_nS; ++sp)
                if ( Val(s,sp,a) > MIN_PROB ) vsp.push_back(sp);
            return vsp;
        }

        //Access the data matrix.  Returns a reference for getting and setting.
        inline double& Val(int s, int sp, int a) { return _d[Index(s,sp,a)]; }
        inline double Val(int s, int sp, int a) const { return _d[Index(s,sp,a)]; }
        inline double ValM(int s, int sp, int a) const
        {
            return _d[Index(s,sp,a)]>MIN_PROB ? _d[Index(s,sp,a)] : 0.0;
        }

    private:
        inline int Index(int s, int sp, int a) const
        {
#ifdef DN_DEBUG
            assert(a*_nS*_nS + sp*_nS + s < _nS*_nS*_nA && "Bad index value");
#endif
            //Memory laid out as nA nSxnS planes, column-major so that s varies by row,
            //sp varies by column
            return a*_nS*_nS + sp*_nS + s;
        }

        double* const _d;
        const int _nA, _nS;
};

#endif // TRANSPROB

