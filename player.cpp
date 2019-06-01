#include "player.hpp"

int Player::RandU2(int nMin, int nMax)
{
	return rand() % (nMax - nMin + 1) + nMin;
}

int Player::RandomInt(int a, int b)
{
    float  random = (float)rand()/((float)RAND_MAX+1);
	int  diff = b - a;
	int  r = (int)(random * diff);
	return a + r;
}


void Player::init()
{
        //int xp=(int)(gconst.HEIGHT/2)+RandomInt((int)(-gconst.HEIGHT/2.5f),(int)(gconst.HEIGHT/2.5f));
        //int yp=(int)(gconst.WIDTH/2)+RandomInt((int)(-gconst.HEIGHT/2.5f),(int)(gconst.HEIGHT/2.5f));
       // int zp=(int)(gconst.WIDTH/2)+RandomInt((int)(-gconst.HEIGHT/2.5f),(int)(gconst.HEIGHT/2.5f));
        int xp=RandomInt((int)(-gconst.HEIGHT/1.25f),(int)(gconst.HEIGHT/1.25f));
        int yp=RandomInt((int)(-gconst.HEIGHT/1.25f),(int)(gconst.HEIGHT/1.25f));
        int zp=RandomInt((int)(-gconst.HEIGHT/1.25f),(int)(gconst.HEIGHT/1.25f));

        currPos=Vec3(xp,yp,zp);

        float LO=-1.f;
        float HI=1.0f;
        float xv=LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        float yv=LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        float zv=LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

        type=RandomInt(0,gconst.TypeSize);
        velocity=Vec3(xv,yv,zv);

        for (int l=0;l<gconst.TypeSize;l++) {
            typeAmount[l]=0;
            for (int l1=0;l1<gconst.TypeSize;l1++) {
                typeAmountPP[l][l1]=0;
            }
        }
}

int Player::forward()
{

}

void Player::update_field()
{

}



