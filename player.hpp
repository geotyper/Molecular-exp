#pragma once

#include <random>
#include "log.hpp"
#include "game_support.hpp"
#include "constant.hpp"
#include <vector>
#include <array>
#include <algorithm>
#include "vec3.h"


using namespace std;
using namespace logp;


struct Player
{
	int RandU2(int nMin, int nMax);
	int RandomInt(int a, int b);

	//bot main
    int id=0;
    int index=0;

    Player(int _id,  GameConstants _gconst) : id(_id), gconst(_gconst) {}
    //Player(int _id, Grid& map, GameConstants gconst, int _expand);

    GameConstants gconst;

    bool active=true;
    bool collide=false;
    bool free=true;
    bool link=false;

    float radius=1.0f;
    float mass=5.0f;
    int type=1;

    int typeAmount[3];
    int typeAmountPP[3][3];

    Vec3 currPos= Vec3(0, 0, 0);
    Vec3 oldPos= Vec3(0, 0, 0);
    Vec3 force= Vec3(0, 0, 0);
    Vec3 velocity= Vec3(0, 0, 0);
    Vec3 velocity_old= Vec3(0, 0, 0);
    Vec3 nvelocity= Vec3(0, 0, 0);

    Vec3 accelerate= Vec3(0, 0, 0);
    Vec3 nvaccelerate= Vec3(0, 0, 0);

    int forward();

    void init();
    void update_field();


    // bot logics
    Storage storage;

};
