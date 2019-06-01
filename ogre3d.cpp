#include "ogre3d.hpp"


Graph3D::Graph3D() : OgreBites::ApplicationContext("OgreExp01")
{

}
Graph3D::~Graph3D()
{
}
//! [constructor]

//! [key_handler]
bool Graph3D::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    int key = evt.keysym.sym;

    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        ExitLoop = false;
        //	getRoot()->queueEndRendering();
        getRoot()->shutdown();
    }
    if (evt.keysym.sym == OgreBites::SDLK_SPACE)
    {
        ExitLoop = false;
        //	getRoot()->queueEndRendering();

        //	getRoot()->destroySceneManager(scnMgr);
        //	getRoot()->shutdown();
    }

    if (key == 'h' || key == OgreBites::SDLK_F1)   // toggle visibility of help dialog
    {
        if (!mTrayMgr->isDialogVisible()) mTrayMgr->showOkDialog("Help", "No cyrillic type");
        else mTrayMgr->closeDialog();
    }

    if (key == 'a')   // toggle visibility of help dialog
    {
        gconstChange=true;
        gconst.constrLenth+=0.1f;
        if(gconst.constrLenth>gconst.constrLenthCreate-1.0f)
            gconst.constrLenth=gconst.constrLenthCreate-1.0f;
    }
    if (key == 'z')   // toggle visibility of help dialog
    {
        gconstChange=true;
        gconst.constrLenth-=0.1f;
        if(gconst.constrLenth<0.15f)
            gconst.constrLenth=0.15f;
    }

    if (key == 's')   // toggle visibility of help dialog
    {
        gconstChange=true;
        gconst.constrLenthCreate+=0.1f;
        if(gconst.constrLenthCreate>gconst.constrLenthCreateMax)
            gconst.constrLenthCreate=gconst.constrLenthCreateMax;
    }
    if (key == 'x')   // toggle visibility of help dialog
    {
        gconstChange=true;
        gconst.constrLenthCreate-=0.1f;
        if(gconst.constrLenthCreate<gconst.constrLenth+2.0f)
            gconst.constrLenthCreate=gconst.constrLenth+2.0f;
    }

    if (key == 'f')   // toggle visibility of help dialog
    {
        if (pauseRender)
        {
            nButton->setCaption("On Graphics");
            pauseRender = false;
        }
        else
        {
            pauseRender = true;
            nButton->setCaption("Off Graphics");
        }
    }

    if (key == 'i')   // toggle visibility of help dialog
    {
        if (showStat)
        {
            nLabel->hide();
            nTextTick->hide();
            showStat = false;
        }
        else
        {
            nTextTick->show();
            nLabel->show();
            showStat = true;
        }
    }

    if (key == 'c')   // toggle visibility of help dialog
    {
        if (selectbyMouse)
        {

            selectbyMouse = false;
        }
        else
        {
           selectbyMouse = true;
        }
    }

    if (key == 'q')   // toggle visibility of help dialog
    {
        exitGraphLoop= true;
    }
    if (key == 'p')   // toggle visibility of help dialog
    {
        gameTick -= 500;
        if (gameTick < 300)
            gameTick = 500;
    }
    return true;
}



/*
void Graph3D::buttonHit(OgreBites::Button* button)
{
    if (button->getName()==("Button01"))
    {
        if (pauseRender)
        {
            button->setCaption("Off Graphics");
            pauseRender = false;
        }
        else
        {
            pauseRender = true;
            button->setCaption("On Graphics");
        }
    }
}
*/

/*
bool Graph3D::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    if (mTrayMgr->mousePressed(evt)) return true;

    return true;
}
*/


bool Graph3D::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (mTrayMgr->mouseMoved(evt)) return true;
    /* normal mouse processing here... */
    return true;
}

bool Graph3D::mousePressed(const OgreBites::MouseButtonEvent& evt)
{

    if (mTrayMgr->mousePressed(evt))
    {
        if (nButton->getState() == OgreBites::ButtonState::BS_DOWN)

        {

            if (drawDebugInfo)
            {
                nButton->setCaption("On Debug Graph");
                drawDebugInfo = false;
            }
            else
            {
                drawDebugInfo = true;
                nButton->setCaption("Off Debug Graph");
            }


        }

    }
    if(evt.button==SDL_BUTTON_LEFT)
    {
        Ogre::String objectName = "";
        Ogre::Ray mouseRay = mTrayMgr->getCursorRay(mCamera);

        mRaySceneQuery->setRay(mouseRay);
        mRaySceneQuery->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
        Ogre::RaySceneQueryResult::iterator itr;

        Ogre::SceneNode* mCurrentObject;
        //logp::log("Find object name  ");
        for ( itr = result.begin(); itr != result.end(); itr++ )
            {
                if ( itr->movable )
                {
                    mCurrentObject =itr->movable->getParentSceneNode();
                    Ogre::ColourValue c1=Ogre::ColourValue(0.75f,0.15f,0.15f,0.0);
                                //SphereBotList[i].m_pMat->getTechnique(0)->getPass(0)->setEmissive(SphereBotList[i].EmissiveAlfa);
                    nameClickObject=mCurrentObject->getAttachedObject(0)->getName();
                    if(nameClickObject.front()=='b' and selectbyMouse)
                    {
                        Ogre::Entity* mEntity1 = static_cast<Ogre::Entity*>(mCurrentObject->getAttachedObject(0));
                        mEntity1->getSubEntity(0)->getTechnique()->getPass(0)->setEmissive(c1);
                    }
                    //logp::log(mCurrentObject->getAttachedObject(0)->getName());
                    break;
                } // if
             } // for
   }
    return true;
}

void Graph3D::showInfoWidget(int WTick)
{
    //nLabel->setCaption(" NN: "+std::to_string(timeNN)+" PhEn: " + std::to_string(timeNN)+
    //	 " Ogre: " + std::to_string(timeNN));
//	nLabelTick->setCaption("Tick: " + std::to_string(Tick));

    nTextTick->setText(
        " Lenth Create: " + std::to_string(gconst.constrLenthCreate) + "\n" +
        " Constr Lenth: " + std::to_string(gconst.constrLenth) + "\n" +
        " Tick: " + std::to_string(WTick)+ "\n" +
        " Select by Mouse: " + std::to_string(selectbyMouse));

    nTextTick->setTextAlignment(Ogre::TextAreaOverlayElement::Alignment::Left);
    nTextTick->setCaption("Actual Units Speed");

    mTrayMgr->refreshCursor();
    //	nTextTick->show();
}

bool Graph3D::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mTrayMgr->frameRendered(evt);
    return true;
}



void Graph3D::setup()
{
    gameTick = 1;
    // do not forget to call the base first
    OgreBites::ApplicationContext::setup();
    // rootWindow = getRenderWindow();
    rootWindow = getRenderWindow();

    // register for input events
    addInputListener(this);

    // get a pointer to the already created root
    root = getRoot();
    scnMgr = root->createSceneManager();
    scnMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    windowResized(rootWindow);

    Ogre::uint32 xx = 1800;
    Ogre::uint32 yy = 1000;

    //rootWindow->setFullscreen(true, xx, yy);
    //Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");

    // register our scene with the RTSS
    shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);
   // Ogre::Plane plane(Ogre::Vector3::UNIT_Y, Ogre::Vector3(-150,-100,-150));
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -132); //-92
    Ogre::MeshManager::getSingleton().createPlane(
        "ground",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        2 * 700, 2 * 700, 20, 20,
        true,
        1.0, -1.0, 5,
        Ogre::Vector3::UNIT_Z);

    Ogre::Entity* groundEntity = scnMgr->createEntity("ground");

    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
    groundEntity->setCastShadows(true);
    scnMgr->getRootSceneNode()->getChild(0)->setPosition(100,-130,100);
    //groundEntity->setMaterialName("SimpleGround");

    groundEntity->setMaterialName("SimpleColorDarkGrey");
    Ogre::ColourValue fadeColour(0.79, 0.79, 0.79);
    //scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 10, 1520);
    //scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 27,77);
    // without light we would just get a black screen

    Ogre::Light* light = scnMgr->createLight("MainLight");
    light->setCastShadows(true);
    light->setSpecularColour(fadeColour);
    lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(1950, 11850, 1350);
    lightNode->attachObject(light);

    // also need to tell where we are
    camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(50, 550, 50);
    camNode->lookAt(Ogre::Vector3(150, -90, 150), Ogre::Node::TS_PARENT);

    // create the camera
    mCamera = scnMgr->createCamera("myCam");
    mCamera->setNearClipDistance(5); // specific to this sample
    mCamera->setAutoAspectRatio(true);

    camNode->attachObject(mCamera);

    camman = new OgreBites::CameraMan(camNode);
    camman->setStyle(OgreBites::CS_ORBIT);
    camman->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(0.3), 125);
   // camman->setYawPitchDist(Ogre::Radian(0.1), Ogre::Radian(0.15), 625);
    addInputListener(camman);

    Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("tutorialFont1", "General");
    mFont->setType(Ogre::FT_TRUETYPE);
    mFont->setSource("ctrl_freak.ttf");
    mFont->setParameter("size", "21");
    mFont->setParameter("resolution", "96");
    mFont->load();

    // and tell it to render into the main window
    Ogre::Viewport *mViewport = getRenderWindow()->addViewport(mCamera);
    mCamera->setAspectRatio((Ogre::Real)mViewport->getActualWidth() / (Ogre::Real)mViewport->getActualHeight());
    mCamera->setAutoAspectRatio(true);

   // Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();


    mTrayMgr = new OgreBites::TrayManager("InterfaceName", rootWindow, this);
    //mTrayMgr->showBackdrop("SdkTrays/Bands");
    scnMgr->addRenderQueueListener(getOverlaySystem());

    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->showCursor();
    nButton = mTrayMgr->createButton(OgreBites::TL_BOTTOMRIGHT, "Button01", "Off Graphics", 125.0);

    ctrls = new OgreBites::AdvancedRenderControls(mTrayMgr, mCamera);
    addInputListener(ctrls);


    if (showStat)
    {

       // nLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "Info", "Graph Server for PaperIO", 500.0);
        //nLabelTick = mTrayMgr->createLabel(OgreBites::TL_TOP, "InfoTick", "Tick: ", 300.0);
        nTextTick = mTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "InfoTextBox", "", 250.0, 155.f);
        //mTrayMgr->createTextBox(OgreBites::TL_TOPLEFT, "Info", "Tick:   NN time: ", 125.f,65.f);
    }

    mRaySceneQuery = scnMgr->createRayQuery(Ogre::Ray());
    mRaySceneQuery->setSortByDistance(true);

}

bool Graph3D::InitSphereList(std::vector<Player>& bots, std::vector<Constraint>& constrains)
{
    vector<Ogre::ColourValue> colorlist;
    for (int i=0;i<gconst.TypeSize+1; i++)
    {
        Ogre::ColourValue ac=Ogre::ColourValue(RandomInt(30,185)/255.f,RandomInt(30,185)/255.f, RandomInt(30,185)/255.f, 1.0f);
        colorlist.push_back(ac);
    }

    int colorBands = 125;
    int colorFields = 175;

    for (int i = 0; i < bots.size(); i++)
    {
        SphereBot bot;
        bot.SphereNode = scnMgr->getRootSceneNode()->createChildSceneNode();
        //Ogre::Entity* mSphere1 = scnMgr->createEntity("botSphere" + std::to_string(i), Ogre::SceneManager::PT_SPHERE);
        //	Ogre::Entity* mSphere1 = scnMgr->createEntity("mySphere" + std::to_string(i), "Sphere02.mesh");
        Ogre::Entity* mSphere1 = scnMgr->createEntity("botSphere" + std::to_string(i), "SphereLP.mesh"); //spherer1.mesh
        mSphere1->setCastShadows(true);
        mSphere1->setMaterialName("SimpleColorDarkGrey");
        Ogre::MaterialPtr  MaterialBluePtr = mSphere1->getSubEntity(0)->getMaterial();
        Ogre::MaterialPtr  m_pMat_blue = MaterialBluePtr->clone("Player_blue" + std::to_string(i));
        mSphere1->setMaterial(m_pMat_blue);
        //bot.EmissiveAlfa=Ogre::ColourValue((55*bots[i].id)/255.f,(15*bots[i].id)/255.f, (95*bots[i].id)/255.f, 1.0f);

        //bot.EmissiveAlfa=Ogre::ColourValue(RandomInt(30,185)/255.f,RandomInt(30,185)/255.f, RandomInt(30,185)/255.f, 1.0f);
        bot.EmissiveAlfa=colorlist[bots[i].type];
        bot.SphereEntity=mSphere1;
        bot.SphereNode->attachObject(mSphere1);
        float Scale =1.0;
        //float Scale = bots[i].radius*0.06;
        bot.SphereNode->setPosition(Ogre::Vector3(bots[i].currPos.x*step, bots[i].currPos.y*step,bots[i].currPos.z*step));
        bot.SphereNode->setScale(Ogre::Vector3(Scale, Scale, Scale));

        bot.SphereNode->setVisible(false);
        SphereBotList.push_back(bot);
/*
        ObjectTextDisplay* temptextBot = new ObjectTextDisplay(mSphere1, mCamera);
        temptextBot->enable(true);
        temptextBot->setText("Bot Id " + to_string(bots[i].id));
        textBot.push_back(temptextBot);
*/
    }

    Ogre::Entity* mSphereA = scnMgr->createEntity("zoneSphereA" , "spherer1.mesh");
    mSphereA->setMaterialName("SimpleColorDarkGrey");
    Ogre::MaterialPtr  MaterialBluePtrA= mSphereA->getSubEntity(0)->getMaterial();

    deque<Ogre::Vector3> somePoints;
    for(auto constrain : constrains)
    {
        Vec3 a1=bots[constrain.particleA].currPos;
        Vec3 b1=bots[constrain.particleB].currPos;
        somePoints.push_back(Ogre::Vector3(a1.x,a1.y,a1.z));
        somePoints.push_back(Ogre::Vector3(b1.x,b1.y,b1.z));
    }

    DynamicLines * lines = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
    for (int i = 0; i < somePoints.size(); i++) {
        lines->addPoint(somePoints[i]);
    }
    lines->update();
    Ogre::SceneNode *linesNode = scnMgr->getRootSceneNode()->createChildSceneNode("lines");
    linesNode->attachObject(lines);


    for (int i = 0; i < bots.size(); i++)
    {
        if(bots[i].active)
        {   Ogre::ColourValue c1=Ogre::ColourValue(0.15f,0.15f,0.15f,0.0)+SphereBotList[i].EmissiveAlfa;
            //SphereBotList[i].m_pMat->getTechnique(0)->getPass(0)->setEmissive(SphereBotList[i].EmissiveAlfa);
            SphereBotList[i].SphereEntity->getSubEntity(0)->getTechnique()->getPass(0)->setEmissive(c1);
                    //->getTechnique(0)->getPass(0)->setEmissive(RandomInt(0,225)/255.f,0.1, 0.1);
            SphereBotList[i].SphereNode->setVisible(true);
        }
        else
        {
            SphereBotList[i].SphereNode->setVisible(false);
        }
    }

    return true;
}

bool Graph3D::DrawSphere(std::vector<Player>& bots, std::vector<Constraint>& constrains)
{

    for (int i = 0; i < bots.size(); i++)
    {
        if(bots[i].active)
        {
            //Ogre::ColourValue c1=Ogre::ColourValue(0.15f,0.15f,0.15f,0.0)+SphereBotList[i].EmissiveAlfa;
            //SphereBotList[i].m_pMat->getTechnique(0)->getPass(0)->setEmissive(SphereBotList[i].EmissiveAlfa);
           // SphereBotList[i].SphereEntity->getSubEntity(0)->getTechnique()->getPass(0)->setEmissive(c1);
                    //->getTechnique(0)->getPass(0)->setEmissive(RandomInt(0,225)/255.f,0.1, 0.1);
            SphereBotList[i].SphereNode->setVisible(true);
        }
        else
        {
            SphereBotList[i].SphereNode->setVisible(false);
        }
    }

    return true;

}

bool Graph3D::MoveSphere(std::vector<Player>& bots, std::vector<Constraint>& constrains, std::vector<Constraint>& allconstrains)
{

    for (int i = 0; i < bots.size(); i++)
    {
        SphereBotList[i].SphereNode->setPosition(Ogre::Vector3(bots[i].currPos.x*step-gconst.WIDTH/2,
                                                               bots[i].currPos.y*step-gconst.WIDTH/2 ,
                                                               bots[i].currPos.z*step-gconst.HEIGHT/2));
        //textBot[i]->update();
    }

   deque<Ogre::Vector3> somePoints;
   if (drawDebugInfo)
   {
        /*
         for(auto constrain : constrains)
         {
             if(constrain.active==true)
             {
                 Vec3 a1=bots[constrain.particleA].currPos;
                 Vec3 b1=bots[constrain.particleB].currPos;
                 somePoints.push_back(Ogre::Vector3(a1.x*step-gconst.WIDTH/2,a1.y*step-gconst.WIDTH/2,a1.z*step-gconst.WIDTH/2));
                 somePoints.push_back(Ogre::Vector3(b1.x*step-gconst.WIDTH/2,b1.y*step-gconst.WIDTH/2,b1.z*step-gconst.WIDTH/2));
             }


         }
         */
       int indexConstrain1=0;
       for(int botindexA=0;botindexA<gconst.number_of_bots; botindexA++)
       {
           for(int botindexB=0;botindexB<gconst.number_of_bots; botindexB++)
           {
               Constraint constrain= allconstrains[indexConstrain1];
               if(constrain.active==true)
               {
                   Vec3 a1=bots[constrain.particleA].currPos;
                   Vec3 b1=bots[constrain.particleB].currPos;
                   somePoints.push_back(Ogre::Vector3(a1.x*step-gconst.WIDTH/2,a1.y*step-gconst.WIDTH/2,a1.z*step-gconst.WIDTH/2));
                   somePoints.push_back(Ogre::Vector3(b1.x*step-gconst.WIDTH/2,b1.y*step-gconst.WIDTH/2,b1.z*step-gconst.WIDTH/2));
                }
               indexConstrain1++;
           }
       }


    }

    Ogre::SceneNode *lnode = dynamic_cast<Ogre::SceneNode*>(scnMgr->getRootSceneNode()->getChild("lines"));
    DynamicLines *lines = dynamic_cast<DynamicLines*>(lnode->getAttachedObject(0));
/*
    if (lines->getNumPoints() != somePoints.size()) {
            // Oh no!  Size changed, just recreate the list from scratch
            lines->clear();
            for (int i = 0; i < somePoints.size(); ++i) {
                lines->addPoint(somePoints[i]);
            }
     }
     else {
            // Just values have changed, use 'setPoint' instead of 'addPoint'
            for (int i = 0; i < somePoints.size(); ++i) {
                lines->setPoint(i, somePoints[i]);
            }
     }
     */

    lines->clear();
    for (int i = 0; i < somePoints.size(); ++i) {
        lines->addPoint(somePoints[i]);
        //lines->setMaterial(SphereBotList[constrains[i].particleA].SphereEntity->getSubEntity(0)->getMaterial());
    }
    lines->setMaterial(SphereBotList[constrains[0].particleA].SphereEntity->getSubEntity(0)->getMaterial());
    lines->update();


    return true;
}

int Graph3D::RandomInt(int a, int b)
{
    float  random = rand();
    float rd=random/(float)(RAND_MAX);
    int  diff = b - a;
    int  r = (int)(rd * diff);
    return a + r;
}
