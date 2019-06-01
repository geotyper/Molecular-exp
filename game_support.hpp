#pragma once

#include "log.hpp"
#include <vector>
#include <array>

using namespace std;
using namespace logp;



enum ConstraintType
{
    StructSpring,
    ShearSpring,
    BendSpring
};


struct Constraint
{
    int	particleA, particleB;
    float	restlength;
    float   restlenghtSqr;
    float	restlength_init;
    int  type;
    ConstraintType ctype;
    float force=0.f;
    bool active=false;

    bool operator==(const Constraint& rhs) const
        {
            return (particleA==rhs.particleA and particleB==rhs.particleB) or (particleA==rhs.particleB and particleB==rhs.particleA); // or another approach as above
        }

};

enum class Gamemode : int{
        wander = 1,
        square = 2,
        goback = 3,
        main   = 4,
        kill   = 5,
        gohome = 6

    };

enum class Gameturn : char{
        left ='l',
        right ='r',
        none='n'
    };

struct Storage
{
    Gameturn turn;
    Gamemode  mode;
    int  count;
    int  maxl;
};



struct MapInfo
{
    int Fields=0;
    int Bands=0;
};




