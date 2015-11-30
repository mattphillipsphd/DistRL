#ifndef GLOBALS
#define GLOBALS

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef std::unordered_map<int, std::unordered_map<int, double> > Fmap;
    //<action, <statep, val> >

#define MIN_PROB (1e-6)

#endif // GLOBALS

