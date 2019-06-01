#include "SolverGPU.cuh"




void check(cudaError x) {
    fprintf(stderr, "%s\n", cudaGetErrorString(x));
}


typedef unsigned long uint64_t;

void PrintCudaCards()
{
	int  GPU_N;

	printf("Starting MultiGPU\n");
	cudaGetDeviceCount(&GPU_N);

	if (GPU_N > 4)
	{
		GPU_N = 4;
	}

	printf("CUDA-capable device count: %i\n", GPU_N);

}

__global__ void initkernelParticals(Player *bots,int numElements, GameConstants* gconst)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;


    for (int pos = tid; pos < numElements; pos += threadN)
    {
        for (int l=0;l<gconst->TypeSize;l++)
        {
            bots[pos].typeAmount[l]=0;
            for (int l1=0;l1<gconst->TypeSize;l1++)
            {
                bots[pos].typeAmountPP[l][l1]=0;
            }
        }
    }
}

__device__ inline float cLength(Vec3 v)
{
    float lSq = cLengthSq(v);
    if (lSq>0.0f)
        return  sqrtf(lSq);
    else
        return 0.0f;
}


__device__ inline float cLengthSq(Vec3 v)
{
    return Dot(v,v);
}

__device__ inline Vec3 cSafeNormalize(Vec3 v, Vec3 fallback)
{
    float l = cLengthSq(v);
    if (l > 0.0f)
    {
        return v * cInvSqrt(l);
    }
    else
        return fallback;
}

__device__ inline float cDistance(Vec3 v1, Vec3 v2)
{
    return cLength(v1-v2);
}

__device__ inline float cInvSqrt(float x)
{
    return 1.0f/sqrtf(x);
}


__device__ Vec3 projectUonV(const Vec3& u, const Vec3& v)
{
    Vec3 r=Vec3(0,0,0);
    float DotVV=Dot(v, v);
    if(DotVV!=0)
        r = v* (Dot(u, v)/DotVV);
    return r;
}

__device__ void performCollision(Player& s1, Player& s2)
{

        float softCoeff=0.973f;
        Vec3 nv1; // new velocity for sphere 1
        Vec3 nv2; // new velocity for sphere 2
        // this can probably be optimised a bit, but it basically swaps the velocity amounts
        // that are perpendicular to the surface of the collistion.
        // If the spheres had different masses, then u would need to scale the amounts of
        // velocities exchanged inversely proportional to their masses.
        nv1 = s1.velocity;
        nv1 =nv1+ projectUonV(s2.velocity, s2.currPos- s1.currPos);
        nv1 =nv1- projectUonV(s1.velocity, s1.currPos- s2.currPos);
        nv2 = s2.velocity;
        nv2 =nv2+ projectUonV(s1.velocity, s2.currPos- s1.currPos);
        nv2 =nv2- projectUonV(s2.velocity, s1.currPos- s2.currPos);
        s1.velocity = softCoeff*nv1;
        s2.velocity = softCoeff*nv2;
}



__device__ inline int array2D(int a, int b, int numElements)
{
    return a*numElements+b;
}
__global__ void UpdateBallVelocity(Player* bot, Constraint* allconstr, GameConstants* gconst,int numElements)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float m=10.0f;
    float deltaTime=gconst->deltaTime;


    for (int pos = tid; pos < numElements; pos += threadN)
    {
        Vec3 directForce=Vec3(0,0,0);
        for (int i = 0; i< numElements; i++)
        {
          if(pos!=i)
          {
               bool collide=false;
               bot[pos].force=0.0f;
               float dotV2=cDistance(bot[pos].currPos,bot[i].currPos);
               float dotV=cDistance(bot[pos].currPos,bot[i].currPos)-(bot[pos].radius+bot[i].radius);
               float dotVC=cDistance(bot[pos].currPos,Vec3(0,0,0));
               Vec3 directPP=bot[pos].currPos-bot[i].currPos;


               if(dotV<0.0f)
               {
                   collide=true;
               }

              if(dotV<15.0f)
              // if(bot[pos].link)
               {
                   if(LinksField[bot[pos].type][bot[i].type]==1)
                       bot[pos].force-=directPP*gconst->gravconst2*(bot[pos].mass*bot[i].mass)/(dotV2*dotV2+0.001f);
                   else {
                       bot[pos].force+=directPP*gconst->gravconst2*(bot[pos].mass*bot[i].mass)/(dotV2*dotV2+0.001f);
                   }
               }

               if (allconstr[array2D(pos,i,numElements)].active )
                       //or allconstr[array2D(i,pos,numElements)].active)
               {

                   if (dotV>gconst->constrLenth)
                        //bot[pos].force-=directPP*gconst->gravconst*(bot[pos].mass*bot[i].mass)/(1-(dotV2-gconst->constrLenth)/gconst->constrLenth);
                       bot[pos].force-=directPP*gconst->gravconst*(bot[pos].mass*bot[i].mass)/abs(1-(dotV-gconst->constrLenth)/gconst->constrLenth);
                   if(dotV<gconst->constrLenth)
                        //bot[pos].force+=directPP*gconst->gravconst*(bot[pos].mass*bot[i].mass)/(1-(gconst->constrLenth-dotV2)/gconst->constrLenth);
                       bot[pos].force+=directPP*gconst->gravconst*(bot[pos].mass*bot[i].mass)/abs(1-(gconst->constrLenth-dotV)/gconst->constrLenth);



               }
               else
               {

                   if(dotVC>175)
                   {
                        bot[pos].force-=directPP*gconst->gravconst*(bot[pos].mass*bot[pos].mass)/(dotVC*dotVC+0.001f);
                        //bot[pos].velocity=bot[pos].velocity+deltaTime*bot[pos].force;
                   }

               }

               bot[pos].velocity=bot[pos].velocity+deltaTime*bot[pos].force;


               if(collide)
               {
                   //move to real point of collide
                   float dotV2=cDistance(bot[pos].currPos,bot[i].currPos);
                   float dist=0.001+(bot[pos].radius+bot[i].radius)-dotV2;
                   float distHalf=dist/2.f;
                   Vec3 directPP=bot[pos].currPos-bot[i].currPos;
                   bot[pos].currPos=bot[pos].currPos+ distHalf*cSafeNormalize(directPP,Vec3(0,0,0));
                   bot[i].currPos=bot[i].currPos- distHalf*cSafeNormalize(directPP,Vec3(0,0,0));

                   if(!bot[pos].collide)
                   {
                     bot[pos].collide=true;
                     bot[i].collide=true;
                     performCollision(bot[pos],bot[i]);
                     bot[i].force=Vec3(0,0,0);
                     bot[pos].force=Vec3(0,0,0);
                   }
               }

            }
        }



    }
}


__global__ void UpdateBallPos(Player* bot, GameConstants* gconst, int numElements)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float deltaTime=gconst->deltaTime;

    for (int pos = tid; pos < numElements; pos += threadN)
    {
        //float dotV=cDistance(bot[pos].currPos,Vec3(0,0,0));
       // if(dotV>95.0f)
       //     bot[pos].velocity=-0.75f*bot[pos].velocity;

        if(cLength(bot[pos].velocity)>gconst->speedLimit)
        {
            bot[pos].velocity=cSafeNormalize(bot[pos].velocity, Vec3(0,0,0))*gconst->speedLimit;
        }
/*
        int sumlink=0;
        for(int l=0;l<3;l++)
        {
            sumlink+=bot[pos].typeAmount[l];
        }

        if(sumlink>0)
            bot[pos].link=true;
        else {
            bot[pos].link=false;
        }

        */

        //if(abs(bot[pos].currPos.x)>gconst->WIDTH)
        //    bot[pos].velocity.x=-bot[pos].velocity.x;
        //if(abs(bot[pos].currPos.y)>gconst->HEIGHT)
        //    bot[pos].velocity.y=-bot[pos].velocity.y;
        //if(abs(bot[pos].currPos.z)>gconst->DEPTH)
        //    bot[pos].velocity.z=-bot[pos].velocity.z;

        bot[pos].currPos=bot[pos].currPos+deltaTime*bot[pos].velocity;
        bot[pos].collide=false;

        bot[pos].velocity_old=bot[pos].velocity;

    }
}

__global__ void UpdateBallPos2(Player* bot, int numElements)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float m=10.0f;
    float deltaTime=0.01f;

    for (int pos = tid; pos < numElements; pos += threadN)
    {
        //float dotV=cDistance(bot[pos].currPos,Vec3(0,0,0));
       // if(dotV>95.0f)
       //     bot[pos].velocity=-0.75f*bot[pos].velocity;
       // bot[pos].accelerate=bot[pos].force/bot[pos].mass;
        bot[pos].velocity=bot[pos].velocity+deltaTime*bot[pos].accelerate;
        bot[pos].currPos=bot[pos].currPos+deltaTime*bot[pos].velocity;
        bot[pos].collide=false;
        bot[pos].velocity_old=bot[pos].velocity;
        bot[pos].force=Vec3(0, 0, 0);
    }
}

__global__ void UpdateConstrains2(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float m=10.0f;
    float deltaTime=gconst->deltaTime;

    for (int pos = tid; pos < numElements; pos += threadN)
    {
        for (int i = 0; i< numElements; i++)
        {
          if(pos!=i)
          {
               float dotV=cDistance(bot[pos].currPos,bot[i].currPos)-(bot[pos].radius+bot[i].radius);
               allconstr[array2D(pos,i, gconst->number_of_bots)].restlength=dotV;


               if(dotV<gconst->constrLenthCreate && Links[bot[pos].type][bot[i].type]==1
                      // && bot[pos].typeAmount[bot[i].type]<LinkTypeSize[bot[i].type]
                        && bot[pos].typeAmount[bot[i].type]<LinkTypeSize[bot[i].type]
                        && bot[i].typeAmount[bot[pos].type]<LinkTypeSize[bot[pos].type]
                       && bot[pos].typeAmountPP[bot[pos].type][bot[i].type]<LinkTypePP[bot[pos].type][bot[i].type]
                       && bot[i].typeAmountPP[bot[i].type][bot[pos].type]<LinkTypePP[bot[i].type][bot[pos].type]
                       && (allconstr[array2D(pos,i, gconst->number_of_bots)].active!=true or
                           allconstr[array2D(i,pos, gconst->number_of_bots)].active!=true)
                       //&& bot[i].typeAmount[bot[pos].type]<LinkTypeSize[bot[pos].type]
                       )
               {
                   //constr[pos].active=true;
                  // atomicAdd(&bot[pos].typeAmount[bot[i].type],1);
                  // atomicAdd(&bot[i].typeAmount[bot[pos].type],1);
                   bot[pos].typeAmount[bot[i].type]+=1;
                   bot[i].typeAmount[bot[pos].type]+=1;
                   bot[pos].typeAmountPP[bot[pos].type][bot[i].type]+=1;
                   bot[i].typeAmountPP[bot[i].type][bot[pos].type]+=1;
                   allconstr[array2D(pos,i, gconst->number_of_bots)].active=true;
                   allconstr[array2D(i,pos, gconst->number_of_bots)].active=true;

                   bot[pos].link=true;
                   bot[i].link=true;

               }


           }
        }
    }
}
__global__ void UpdateConstrains3(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2)
{
        int  tid = blockIdx.x * blockDim.x + threadIdx.x;
        int  threadN = gridDim.x * blockDim.x;

        float m=10.0f;
        float deltaTime=gconst->deltaTime;
    for (int pos = tid; pos < numElements; pos += threadN)
    {
        for (int i = 0; i< numElements; i++)
        {
          if(pos!=i)
          {
               float dotV=cDistance(bot[pos].currPos,bot[i].currPos)-(bot[pos].radius+bot[i].radius);
               allconstr[array2D(pos,i, gconst->number_of_bots)].restlength=dotV;

              if(dotV>=gconst->constrLenthCreate &&
                     (allconstr[array2D(pos,i, gconst->number_of_bots)].active==true
                      or
                      allconstr[array2D(i,pos, gconst->number_of_bots)].active==true))
               {
 /*
                  if(bot[pos].typeAmount[bot[i].type]>0)
                       atomicAdd(&bot[pos].typeAmount[bot[i].type],-1);
                  if(bot[i].typeAmount[bot[pos].type]>0)
                       atomicAdd(&bot[i].typeAmount[bot[pos].type],-1);
*/

                  if(bot[pos].typeAmount[bot[i].type]>0)
                       bot[pos].typeAmount[bot[i].type]-=1;
                  if(bot[i].typeAmount[bot[pos].type]>0)
                       bot[i].typeAmount[bot[pos].type]-=1;

                  if(bot[pos].typeAmount[bot[i].type]<=0)
                  {
                       bot[pos].typeAmount[bot[i].type]=0;
                       bot[pos].link=false;
                  }
                  if(bot[i].typeAmount[bot[pos].type]<=0)
                  {
                       bot[i].typeAmount[bot[pos].type]=0;
                       bot[i].link=false;
                  }

                  if(bot[pos].typeAmountPP[bot[pos].type][bot[i].type]>0)
                       bot[pos].typeAmountPP[bot[pos].type][bot[i].type]-=1;
                  if(bot[i].typeAmountPP[bot[i].type][bot[pos].type]>0)
                       bot[i].typeAmountPP[bot[i].type][bot[pos].type]-=1;

                  if(bot[pos].typeAmountPP[bot[pos].type][bot[i].type]<=0)
                  {
                       bot[pos].typeAmountPP[bot[pos].type][bot[i].type]=0;

                  }
                  if(bot[i].typeAmountPP[bot[i].type][bot[pos].type]<=0)
                  {
                       bot[i].typeAmountPP[bot[i].type][bot[pos].type]=0;
                       bot[i].link=false;
                  }


                   allconstr[array2D(pos,i, gconst->number_of_bots)].active=false;
                   allconstr[array2D(i,pos, gconst->number_of_bots)].active=false;

               }
           }
        }
    }

}

__global__ void UpdateConstrains(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float m=10.0f;
    float deltaTime=gconst->deltaTime;

    for (int pos = tid; pos < numElements; pos += threadN)
    {

       float dotV=cDistance(bot[constr[pos].particleA].currPos, bot[constr[pos].particleB].currPos)-(bot[constr[pos].particleA].radius+bot[constr[pos].particleB].radius);
       constr[pos].restlength=dotV;
/*
       if (pos == 0)
       {
           for (int i = 0; i < 3; ++i)
              printf(" particals type %1d %5d ", i, bot[constr[pos].particleA].typeAmount[i]);

            printf("\n");
       }
*/
       if(dotV<5.5f && (Links[bot[constr[pos].particleA].type][bot[constr[pos].particleB].type]==1) &&
            bot[constr[pos].particleA].typeAmount[bot[constr[pos].particleB].type]<LinkTypeSize[bot[constr[pos].particleB].type] &&
            bot[constr[pos].particleB].typeAmount[bot[constr[pos].particleA].type]<LinkTypeSize[bot[constr[pos].particleA].type]
               )
       {

           constr[pos].active=true;
           bot[constr[pos].particleA].typeAmount[bot[constr[pos].particleB].type]+=1;
           bot[constr[pos].particleB].typeAmount[bot[constr[pos].particleA].type]+=1;
           allconstr[array2D(constr[pos].particleA,constr[pos].particleB, numElements2)].active=true;
           allconstr[array2D(constr[pos].particleB,constr[pos].particleA, numElements2)].active=true;

       }
       else
       {
           if(bot[constr[pos].particleA].typeAmount[bot[constr[pos].particleB].type]>0)
            bot[constr[pos].particleA].typeAmount[bot[constr[pos].particleB].type]-=1;
           if(bot[constr[pos].particleB].typeAmount[bot[constr[pos].particleA].type]>0)
            bot[constr[pos].particleB].typeAmount[bot[constr[pos].particleA].type]-=1;

           constr[pos].active=false;
           allconstr[array2D(constr[pos].particleA,constr[pos].particleB, numElements2)].active=false;
           allconstr[array2D(constr[pos].particleB,constr[pos].particleA, numElements2)].active=false;
       }

    }
}

__global__ void CalcConstrains(Player* bot, Constraint* constr, int numElements)
{
    int  tid = blockIdx.x * blockDim.x + threadIdx.x;
    int  threadN = gridDim.x * blockDim.x;

    float m=10.0f;
    float deltaTime=0.01f;
    float gravconst=10.0f;

    for (int pos = tid; pos < numElements; pos += threadN)
    {
       //float dotV=cDistance(bot[constr[pos].particleA].currPos, bot[constr[pos].particleB].currPos);
       float dotV=constr[pos].restlength;
       bool collide=false;
       float sumForce=0.0f;
       Vec3 directForce=Vec3(0,0,0);

       if(dotV<(bot[constr[pos].particleA].radius+bot[constr[pos].particleB].radius))
       {
           collide=true;
       }
       else
       {
           int botTypeForceFlag=1.0f;
           if(bot[constr[pos].particleA].type==bot[constr[pos].particleB].type)
               botTypeForceFlag=-1.0f;

           if(dotV>55.0f)
           {
                sumForce=-gravconst*(bot[constr[pos].particleA].mass*bot[constr[pos].particleB].mass)/(dotV*dotV);
           }
           else if(dotV<35.5f and dotV>9.5f)
           {
                sumForce=-botTypeForceFlag*gravconst*(bot[constr[pos].particleA].mass*bot[constr[pos].particleB].mass)/(dotV*dotV);
           }
           else
           {
                sumForce=gravconst*(bot[constr[pos].particleA].mass*bot[constr[pos].particleB].mass)/(dotV*dotV);
           }

       }

       if(collide)
       {
           //move to real point of collide
           float dist=0.001+(bot[constr[pos].particleA].radius+bot[constr[pos].particleB].radius)-dotV;
           float distHalf=dist/2.f;
           Vec3 directPP=bot[constr[pos].particleA].currPos-bot[constr[pos].particleB].currPos;
           bot[constr[pos].particleA].currPos=bot[constr[pos].particleA].currPos+ distHalf*cSafeNormalize(directPP,Vec3(0,0,0));
           bot[constr[pos].particleB].currPos=bot[constr[pos].particleB].currPos+ distHalf*cSafeNormalize(-directPP,Vec3(0,0,0));


           bot[constr[pos].particleA].collide=true;
           bot[constr[pos].particleB].collide=true;
           performCollision(bot[constr[pos].particleA],bot[constr[pos].particleB]);
           directForce=Vec3(0,0,0);
           sumForce=0;


       }

       Vec3 directPP=bot[constr[pos].particleA].currPos-bot[constr[pos].particleB].currPos;
       bot[constr[pos].particleA].force=bot[constr[pos].particleA].force+sumForce*cSafeNormalize(directPP,Vec3(0,0,0));
       bot[constr[pos].particleB].force=bot[constr[pos].particleB].force-sumForce*cSafeNormalize(directPP,Vec3(0,0,0));


    }
}


void initTrust(thrust::host_vector<int>& a)
{

        thrust::device_vector<int> D(10, 1);
        thrust::copy(a.begin(), a.end(), D.begin());

        // print D
        for(int i = 0; i < D.size(); i++)
            std::cout << "D[" << i << "] = " << D[i] << std::endl;


}

void initKernelParticals(Player *bots,int numElements, GameConstants* gconst)
{
    initkernelParticals<< < int(1 + numElements/ 32), 64 >> > ( bots, numElements, gconst);
    gpuAssert(cudaThreadSynchronize());
}


void updateBallVelocity(Player* bot, Constraint* allconstr,GameConstants* gconst,int numElements)
{
    UpdateBallVelocity<< < int(1 + numElements / 32), 64 >> > (bot, allconstr, gconst, numElements);
    gpuAssert(cudaThreadSynchronize());
    UpdateBallPos << < int(1 + numElements / 32), 64 >> > (bot, gconst, numElements);
    gpuAssert(cudaThreadSynchronize());
}

void updateConstrains(Player* bot, Constraint* constr, Constraint* allconstr, GameConstants* gconst, int numElements, int numElements2)
{
    UpdateConstrains2 << < int(1 + numElements/ 32), 64 >> > (bot, constr, allconstr, gconst, numElements, numElements2);
    gpuAssert(cudaThreadSynchronize());
    UpdateConstrains3 << < int(1 + numElements/ 32), 64 >> > (bot, constr, allconstr, gconst, numElements, numElements2);
    gpuAssert(cudaThreadSynchronize());

}

