#pragma once

#include <string>

struct GameConstants {
    
    int MAX_TURNS = 1;
    int MAX_TIME = 170000;
    int TURNS = 170000;
    int TIMES = 90;

    int WIDTH =  54;
    int HEIGHT = 54;
    int DEPTH =  54;

    int number_of_bots=1710;
    int TypeSize=3;

    float deltaTime=0.0075f;
    float constrLenthCreate=9.5f;
    float constrLenth=6.5f;
    float constrLenthCreateMax=45.5f;

    float gravconst=4.1f;
    float gravconst2=4.1f;

    float speedLimit=15.0f;

    float dragCoefficient      =  0.55f;
    float frictionCoefficient  =  0.35f;
    float frictionCoefficient2  =  0.735f;
    float linearStiffnessCoefficient = 5.0555f;
    float linearStructStiffnessCoefficient = 15.2555f;
    float linearShearStiffnessCoefficient =  5.1555f;
    float linearBendStiffnessCoefficient =7.1555f;

};

