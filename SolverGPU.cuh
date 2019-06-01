#pragma once

#include <cstdio>
#include <cuda_runtime.h>
#include <time.h>
#include <vector>
#include <random>
#include <cstring>
#include <algorithm>


#include "match.hpp"
#include "game_support.hpp"
#include "constant.hpp"
#include "player.hpp"

#include <iostream>

#include <cassert>

#include <device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>


using namespace std;
using namespace logp;

#ifndef gpuAssert

#define gpuAssert( condition ) {if( (condition) != 0 ) { fprintf( stderr, "\n FAILURE %d in %s, line %d\n", condition, __FILE__, __LINE__ );exit( 1 );}}

#endif


void PrintCudaCards();

inline void gpuAssert2(cudaError_t code, const char *file, int line, bool abort=true);


__device__ inline float cLength(Vec3 v);
__device__ inline float cLengthSq(Vec3 v);
__device__ inline Vec3 cSafeNormalize(Vec3 v, Vec3 fallback);
__device__ inline float cDistance(Vec3 v1, Vec3 v2);
__device__ inline float cInvSqrt(float x);

__device__ inline int array2D(int a, int b, int numElements);

__device__ Vec3 projectUonV(const Vec3& u, const Vec3& v);
__device__ void performCollision(Player& s1, Player& s2);


//__device__ static int Links[3][3] = {{0,0,1},{1,0,0},{1,0,0}};
//__device__ static int LinksSize[3][3] = {{0,0,1},{1,0,1},{0,1,0}};

/*
__device__ static int Links[3][3] =      {{1,0,1},{1,0,0},{1,1,0}};
__device__ static int LinksField[3][3] = {{0,0,1},{0,0,0},{0,0,0}};
__device__ static int LinkTypeSize[3] = {2,6,4};
__device__ static int LinkTypePP[3][3] = {{0,2,2},{2,4,2},{2,2,3}};
*/
__device__ static int Links[3][3] =      {{1,0,1},{1,0,0},{1,1,0}};
__device__ static int LinksField[3][3] = {{0,0,1},{0,0,0},{0,0,0}};
__device__ static int LinkTypeSize[3] =  {3,9,2};
__device__ static int LinkTypePP[3][3] = {{0,1,2},{2,6,1},{0,1,0}};

 __global__ void initkernelParticals(Player *bots,int numElements, GameConstants* gconst);

__global__ void UpdateBallVelocity(Player* bot, Constraint* allconstr,GameConstants* gconst,int numElements);

__global__ void UpdateBallPos(Player* botList, GameConstants* gconst,int numElements);
__global__ void UpdateBallPos2(Player* botList, int numElements);

__global__ void UpdateConstrains(Player* bot, Constraint* constr, Constraint* allconstr,GameConstants* gconst,int numElements, int numElements2);
__global__ void UpdateConstrains2(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2);
__global__ void UpdateConstrains3(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2);

__global__ void CalcConstrains(Player* bot, Constraint* constr, int numElements);

void check(cudaError x);

void initKernelParticals(Player *bots,int numElements, GameConstants* gconst);
void updateBallVelocity(Player* bot, Constraint* allconstr,GameConstants* gconst,int numElements);
void updateConstrains(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2);


void initTrust(thrust::host_vector<int>& a);
