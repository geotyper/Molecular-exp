#ifndef OGRE3D_HPP
#define OGRE3D_HPP

#endif // OGRE3D_HPP

#pragma once

#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreTrays.h>
#include "OgreCameraMan.h"
#include "OgreAdvancedRenderControls.h"
#include "player.hpp"
#include "constant.hpp"
#include "game_support.hpp"
#include "DynamicLines.h"
#include "DynamicRenderable.h"
#include <iostream>

#include <SDL2/SDL.h>

#include "deque"
#include <random>


struct SphereBot
{
    int mainId;
    int subId;
    Ogre::SceneNode* SphereNode;
    Ogre::Entity* SphereEntity;
    std::string name;
    Ogre::Vector3 Emissive;
    Ogre::Vector3 Ambient;
    Ogre::Vector3 Diffuse;
    Ogre::ColourValue EmissiveAlfa;
};



class Graph3D : public OgreBites::ApplicationContext, public OgreBites::InputListener, public OgreBites::TrayListener
{

public:

    Graph3D();
    virtual ~Graph3D();

    GameConstants gconst;
    int step=3;

    int gameTick;
    int gameGoal = 0;
    int gameGoalSelf = 0;

    string nameClickObject="";
    bool drawDebugInfo = true;
    bool gconstChange=false;
    bool selectbyMouse=false;
    bool exitGraphLoop=false;

    Ogre::Root* root;
    Ogre::SceneManager* scnMgr;
    Ogre::RenderWindow* rootWindow;

    Ogre::RTShader::ShaderGenerator* shadergen;
    OgreBites::TrayManager* mTrayMgr;

    Ogre::SceneNode* lightNode;
    Ogre::SceneNode* camNode;
    Ogre::Camera* mCamera;
    OgreBites::CameraMan *camman;
    OgreBites::AdvancedRenderControls *ctrls;

    Ogre::RaySceneQuery* mRaySceneQuery;

    std::vector<SphereBot> SphereBotList;

    bool InitSphereList(std::vector<Player>& bots, std::vector<Constraint>& constrains);
    bool DrawSphere(std::vector<Player>& bots, std::vector<Constraint>& constrains);
    bool MoveSphere(std::vector<Player>& bots, std::vector<Constraint>& constrains, std::vector<Constraint>& allconstrains);

    OgreBites::Label* nLabel;
    OgreBites::Label*  nLabelTick;
    OgreBites::TextBox* nTextTick;
    OgreBites::Button* nButton;

    int Tick = 0;
    bool ExitLoop = true;
    bool FixPointsChange = false;
    bool unFixPointsChange = true;
    bool showStat = true;
    bool pauseRender = true;
    bool resetGame = false;

    void setup();
    bool keyPressed(const OgreBites::KeyboardEvent& evt);

    void showInfoWidget(int Tick);

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    //bool mouseReleased(const OgreBites::MouseButtonEvent& evt);

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);

    bool mousePressed(const OgreBites::MouseButtonEvent& evt);

    //void buttonHit(OgreBites::Button* button);


    int RandomInt(int a, int b);

};
