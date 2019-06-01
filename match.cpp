#include "match.hpp"


void Match::init()
{

    for(int botindex=0;botindex<gconst.number_of_bots; botindex++)
    {
        Player tempplayer=Player(botindex, gconst);
        tempplayer.init();
        players.push_back(tempplayer);
    }
/*
    for(int botindexA=0;botindexA<gconst.number_of_bots; botindexA++)
    {
        for(int botindexB=0;botindexB<gconst.number_of_bots; botindexB++)
        {
            Constraint tempConstr;
            tempConstr.particleA=botindexA;
            tempConstr.particleB=botindexB;
            tempConstr.active=true;

            if(botindexA!=botindexB)
            {
                if ( std::find(constraints.begin(), constraints.end(), tempConstr) != constraints.end() )
                    continue;
                else
                    constraints.push_back(tempConstr);
            }
        }

    }
*/
    int indexConstrain1=0;
    for(int botindexA=0;botindexA<gconst.number_of_bots; botindexA++)
    {
        for(int botindexB=0;botindexB<gconst.number_of_bots; botindexB++)
        {
            Constraint tempConstr;
            tempConstr.particleA=botindexA;
            tempConstr.particleB=botindexB;
            tempConstr.active=false;
            allconstraints.push_back(tempConstr);
            indexConstrain1++;
        }
    }

    int indexConstrain=0;
    for(int botindexA=0;botindexA<gconst.number_of_bots-1; botindexA++)
    {
        for(int botindexB=botindexA+1;botindexB<gconst.number_of_bots; botindexB++)
        {

            Constraint tempConstr;
            tempConstr.particleA=botindexA;
            tempConstr.particleB=botindexB;
            tempConstr.active=false;

            constraints.push_back(tempConstr);

            indexConstrain++;

        }
    }

    printf("number of constrains  %c \n", (int)(constraints.size()));

}

void Match::clearbotfields(int id)
{

}

void Match::losebots()
{
    for(auto bot_id: losers)
    {
        printf("----- Match over for bot N: %3d \n", bot_id);
        players[bot_id-1].active=false;
        clearbotfields(bot_id);

    }
    losers.clear();

}
