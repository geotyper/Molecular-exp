#pragma once

#include "log.hpp"
#include "player.hpp"
#include <vector>
#include "constant.hpp"
#include "game_support.hpp"

using namespace std;
using namespace logp;


struct Match
{

    Match(int _max_turns, int _num_players,  GameConstants _const) : max_turns(_max_turns), 
                num_players(_num_players), gconst(_const) {}

	int intID;


	int max_turns;
    int num_players;
    GameConstants gconst;

    vector<Player> players;
    vector<Constraint> constraints;
    vector<Constraint> allconstraints;
    
    vector<int> losers;
    vector<int> winners;

    void init();
    void clearbotfields(int id);
    void losebots();
};
