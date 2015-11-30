#ifndef STATE_H
#define STATE_H

#include <globals.h>

class State
{
    public:
        State(int id, const std::vector<int>& back, const Fmap& P, const Fmap& R)
            : _back(back), _id(id), _P(P), _R(R)
        {
        }
        ~State()
        {
        }

        int Id() const { return _id; }
        double Prob(int sp, int a) const
        {
            if (_P.find(a) == _P.cend()) return 0.0;
            if (_P.at(a).find(sp) == _P.at(a).cend()) return 0.0;
            return _P.at(a).at(sp);
        }
        double Rew(int sp, int a) const
        {
            if (_R.find(a) == _R.cend()) return 0.0;
            if (_R.at(a).find(sp) == _R.at(a).cend()) return 0.0;
            return _R.at(a).at(sp);
        }

    private:
        std::vector<int> _back;
        const int _id;
        Fmap _P, _R;
};

#endif // STATE_H
