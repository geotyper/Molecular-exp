#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#endif // EXPERIMENT_HPP

#pragma once

#include <cstdio>
#include <time.h>
#include <vector>
#include <random>
#include <cstring>
#include <algorithm>

#include <cuda_runtime.h>


#include "match.hpp"
#include "game_support.hpp"
#include "constant.hpp"
#include "player.hpp"

#include <iostream>
#include "ogre3d.hpp"

#include "SolverGPU.cuh"

using namespace std;
using namespace logp;


struct Experiment
{

    Experiment(int _max_exp) : max_exp(max_exp){}

    int max_exp=1;
    bool result_exp=false;

    int timerLoop;
    int numElements;


    GameConstants gconst;
    bool start();
};
