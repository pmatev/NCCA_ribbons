#include "GLWindow.h"
#include <iostream>
#include "ngl/Light.h"
#include "ngl/Transformation.h"
#include "ngl/TransformStack.h"
#include "ngl/Material.h"
#include "ngl/NGLInit.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/ShaderLib.h"
#include "math.h"

GLWindow::GLWindow(
                   QWidget *_parent
                  )
                    : QGLWidget( new CreateCoreGLContext(QGLFormat::defaultFormat()), _parent )
{

  // set this widget to have the initial keyboard focus
  setFocus();
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  this->resize(_parent->size());
  // Now set the initial GLWindow attributes to default values
  // Roate is false
  m_rotate = false;
  m_track = false;
  m_dolly = false;
  // mouse rotation values set to 0
  m_mouseX=0;
  m_mouseY=0;

  m_timer = startTimer(30);
  m_time = 0;

  WIREFRAME=false;
  PAUSED = false;
  m_keysPressed.clear();
}

//----------------------------------------------------------------------------------------------------------------------
GLWindow::~GLWindow()
{
  ngl::NGLInit *Init = ngl::NGLInit::instance();
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";

  delete m_light;
  delete m_axis;
  delete m_RM;
  //delete m_cam;
  delete m_myCam;
  delete m_text;

  Init->NGLQuit();

}

//----------------------------------------------------------------------------------------------------------------------
// This virtual function is called once before the first call to paintGL() or resizeGL(),
//and then once whenever the widget has been assigned a new QGLContext.
// This function should set up any required OpenGL context rendering flags, defining VBOs etc.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::initializeGL()
{
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);			   // Grey Background

  //glEnable(GL_DEPTH_TEST);

  ngl::NGLInit *Init = ngl::NGLInit::instance();

  Init->initGlew();
  #ifdef WIN32
    glewInit(); // need a local glew init as well as lib one for windows
  #endif

  m_time=0;

  m_myCam = new Camera(ngl::Vector(2,2,2),ngl::Vector(0,0,0),ngl::Vector(0,1,0), this);
  m_axis = new Axis(0.3, m_myCam);


  m_RM = new RibbonManager(m_myCam,this);

  /* Create Solo Ribbon */
  m_RM->createSolo();
  Ribbon *r = new Ribbon(0,m_RM->createRandColour(), 1000,0.5, m_RM->getBundle(0), this);
  m_RM->getBundle(0)->addRibbon(r);

  /* Build Tree Here */
//  m_RM->createBranch(0, ngl::Vector(-5.0,0.0,0.0), ngl::Vector(5.0,0.0,0.0),0, 100);
//  m_RM->createBranch(ngl::Vector(5.0,0.0,0.0), ngl::Vector(5.0,5.0,0.0),100, 100);
//  m_RM->createBranch(ngl::Vector(5.0,0.0,0.0), ngl::Vector(0.0,5.0,0.0),100, 100);

  /* Create Some Ribbons */
  //m_RM->createRandomRibbons(50,m_RM->getBundle(0));
  m_myCam->follow(m_RM->getBundle(0));

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  /* **** Create Surface Shader ***** */
  shader->createShaderProgram("Surface");


  shader->attachShader("SurfaceVertex",ngl::VERTEX);
  shader->attachShader("SurfaceFragment",ngl::FRAGMENT);

  shader->loadShaderSource("SurfaceVertex", "shaders/Surface.vs");
  shader->loadShaderSource("SurfaceFragment","shaders/Surface.fs");

  shader->compileShader("SurfaceVertex");
  shader->compileShader("SurfaceFragment");

  shader->attachShaderToProgram("Surface","SurfaceVertex");
  shader->attachShaderToProgram("Surface","SurfaceFragment");

  shader->bindAttribute("Surface",0,"inVert");
  shader->bindAttribute("Surface",1,"inUV");
  shader->bindAttribute("Surface",2,"inNormal");

  shader->linkProgramObject("Surface");

  /* *********************************************************** */

  /* **** Create Normal Shader ***** */
  shader->createShaderProgram("normalShader");

  shader->attachShader("normalVertex",ngl::VERTEX);
  shader->attachShader("normalFragment",ngl::FRAGMENT);
  shader->attachShader("normalGeo",ngl::GEOMETRY);

  shader->loadShaderSource("normalVertex","shaders/normal.vs");
  shader->loadShaderSource("normalFragment","shaders/normal.fs");
  shader->loadShaderSource("normalGeo","shaders/normal.gs");

  shader->compileShader("normalVertex");
  shader->compileShader("normalFragment");
  shader->compileShader("normalGeo");

  shader->attachShaderToProgram("normalShader","normalVertex");
  shader->attachShaderToProgram("normalShader","normalFragment");
  shader->attachShaderToProgram("normalShader","normalGeo");

  shader->bindAttribute("normalShader",0,"inVert");

  shader->bindAttribute("normalShader",1,"inUV");

  shader->bindAttribute("normalShader",2,"inNormal");

  shader->linkProgramObject("normalShader");

  shader->use("normalShader");

  shader->setShaderParam1f("normalSize",0.1);
  shader->setShaderParam4f("vertNormalColour",1,1,0,1);
  shader->setShaderParam4f("faceNormalColour",1,0,0,1);

  shader->setShaderParam1i("drawFaceNormals",false);
  shader->setShaderParam1i("drawVertexNormals",true);
  /* *********************************************************** */

  /* **** Create Phong Shader **** */

  shader->createShaderProgram("Phong");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("PhongVertex",ngl::VERTEX);
  shader->attachShader("PhongFragment",ngl::FRAGMENT);
  // attach the source
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
  // compile the shaders
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  // add them to the program
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  // now bind the shader attributes for most NGL primitives we use the following
  // layout attribute 0 is the vertex data (x,y,z)
  shader->bindAttribute("Phong",0,"inVert");
  // attribute 1 is the UV data u,v (if present)
  shader->bindAttribute("Phong",1,"inUV");
  // attribute 2 are the normals x,y,z
  shader->bindAttribute("Phong",2,"inNormal");

  // now we have associated this data we can link the shader
  shader->linkProgramObject("Phong");
  // and make it active ready to load values
  (*shader)["Phong"]->use();


//  ngl::Material m(ngl::GOLD);
//  // load our material values to the shader into the structure material (see Vertex shader)
//  m.loadToShader("material");




  m_light = new ngl::Light(ngl::Vector(-2,2,-2,1),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::POINTLIGHT);
//  m_light = new ngl::Light();
//  m_light->setColour(ngl::Colour(1,1,1));
//  m_light->setPosition(ngl::Vector(0,0,0));
//  m_light->enable();

  m_text = new ngl::Text(QFont("Arial",14));
  m_text->setColour(1,5,-1);

}

//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget has been resized.
// The new size is passed in width and height.
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::resizeGL(
                        int _w,
                        int _h
                       )
{
  // set the viewport for openGL
  glViewport(0,0,_w,_h);

  // now set the era size values as the screen size has changed

  m_myCam->setShape(45,(float)_w/_h,0.05,350,ngl::PERSPECTIVE);

  m_screenWidth = _w;
  m_screenHeight = _h;

  m_text->setScreenSize(_w,_h);

}
//----------------------------------------------------------------------------------------------------------------------

void GLWindow::loadLightToPhongShader(ngl::Light *_light)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();
    ngl::Matrix iv=m_myCam->getProjectionMatrix();
    iv.transpose();
    _light->setTransform(iv);
    // load these values to the shader as well
    _light->loadToShader("light");

}

//----------------------------------------------------------------------------------------------------------------------

void GLWindow::loadMatricesToPhongShader(
                                     ngl::TransformStack &_tx
                                   )
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["Phong"]->use();
  shader->setShaderParam3f("viewerPos",m_myCam->getEye().m_x,m_myCam->getEye().m_y,m_myCam->getEye().m_z);

  ngl::Matrix MV;
  ngl::Matrix MVP;
  ngl::Mat3x3 normalMatrix;
  ngl::Matrix M;
  M=_tx.getCurrentTransform().getMatrix();
  MV=  _tx.getCurrAndGlobal().getMatrix()*m_myCam->getViewMatrix();
  MVP=  MV*m_myCam->getProjectionMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMatrix("MV",MV);
  shader->setShaderParamFromMatrix("MVP",MVP);
  shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMatrix("M",M);
}


//----------------------------------------------------------------------------------------------------------------------
//This virtual function is called whenever the widget needs to be painted.
// this is our main drawing routine
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::paintGL()
{
    if(!PAUSED)
    {
        // clear the screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if(WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
       //glEnable(GL_CULL_FACE);


//          if(TREE)
//          {
//              std::vector<Bundle*> out;

//              out.push_back(m_RM->getBundle(1));
//              out.push_back(m_RM->getBundle(2));
//              m_RM->splitBundle(m_RM->getBundle(0), out) ;
//              m_myCam->followNone();
//          }

        m_myCam->update();
        loadLightToPhongShader(m_light);

        m_RM->updateBundles();

        //m_axis->draw();
        m_RM->drawBundles();

        //m_myCam->set(m_myCam->getEye(),m_testRibbon->getPos(),ngl::Vector(0,1,0));

        /*   Text Drawing    */

        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        QString numBundles=QString("No. Bundles: %1")
                .arg(m_RM->getBundles()->size());

        QString HUD_Left = QString("Left Mouse: Tumble");
        QString HUD_Right = QString("Right Mouse: Dolly");
        QString HUD_N = QString("N: Toggle Normals");
        QString HUD_W("W: Toggle Wireframe");
        QString HUD_1("1: Phong Shading");
        QString HUD_2("2: Constant Shading");
        QString HUD_Space("Space: Add new ribbon");
        QString HUD_Arrow("ArrowKeys: Move ribbon");
        QString HUD_Enter("Enter: Branch ");
        QString HUD_Tab("Tab: Switch bundle");
        QString HUD_R("R: Reset");
        QString HUD_help = QString("H: Toggle help");

        m_text->setColour(1,1,1);

        if(HUD)
        {
            m_text->renderText(0,0,HUD_N);
            m_text->renderText(0,20,HUD_W);
            m_text->renderText(0,40,HUD_1);
            m_text->renderText(0,60,HUD_2);
            m_text->renderText(0,80,HUD_Space);
            m_text->renderText(0,100,HUD_Arrow);
            m_text->renderText(0,120,HUD_Enter);
            m_text->renderText(0,140,HUD_Tab);
            m_text->renderText(0,160,HUD_Left);
            m_text->renderText(0,180,HUD_Right);
            m_text->renderText(0,200,HUD_R);
            m_text->renderText(0,240,numBundles);
        }
        m_text->renderText(0,m_screenHeight-20,HUD_help);
    }

}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseMoveEvent (
                               QMouseEvent * _event
                              )
{
    // note the method buttons() is the button state when event was called
    // this is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated

    m_mouseX = _event->x();
    m_mouseY = _event->y();
    // Left Mouse Tumble
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
      m_myCam->tumble(m_oldMouseX, m_oldMouseY, m_mouseX, m_mouseY);
    }

    // Middle Mouse Track
    else if(m_track && _event->buttons() == Qt::MidButton)
    {
        m_myCam->track(m_oldMouseX, m_oldMouseY, m_mouseX, m_mouseY);
    }

    // Right Mouse Dolly
    else if(m_dolly && _event->buttons() == Qt::RightButton)
    {
        m_myCam->dolly(m_oldMouseX, m_mouseX);
    }


    m_oldMouseX = m_mouseX;
    m_oldMouseY = m_mouseY;

}


//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mousePressEvent (
                                QMouseEvent * _event
                               )
{

    m_oldMouseY = _event->y();
    m_oldMouseX = _event->x();

    if(_event->button() == Qt::LeftButton)
    {
        m_rotate =true;
    }

    else if(_event->button() == Qt::MidButton)
    {
        m_track = true;
    }

    else if(_event->button() == Qt::RightButton)
    {
        m_dolly=true;
    }

}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::mouseReleaseEvent (
                                  QMouseEvent * _event
                                 )
{
  // this event is called when the mouse button is released
  // we then set Rotate to false
  if (_event->button() == Qt::LeftButton)
  {
      m_rotate=false;
  }
  else if(_event->button() == Qt::MidButton)
  {
      m_track = false;
  }
	// right mouse translate mode
  if (_event->button() == Qt::RightButton)
  {
      m_dolly=false;
  }

  m_oldMouseX = m_mouseX;
  m_oldMouseY = m_mouseY;
}

//----------------------------------------------------------------------------------------------------------------------
void GLWindow::wheelEvent(QWheelEvent *_event)
{

//	// check the diff of the wheel position (0 means no change)
//    if(_event->delta() < 0)
//	{
//        m_myCam->setRadius(m_myCam->getRadius()+ZOOM);
//        //m_myCam->tumble(0,0,0,0);
//        //m_modelPos.m_z+=ZOOM;
//        std::cout<<"zooming oushowNormalt: "<<m_myCam->getRadius()<<std::endl;
//	}
//    else if(_event->delta() > 0 )
//	{
//        m_myCam->setRadius(m_myCam->getRadius()-ZOOM);
//        std::cout<<"zooming in: "<<m_myCam->getRadius()<<std::endl;
//        //m_modelPos.m_z-=ZOOM;
//	}
//	updateGL();
}


//----------------------------------------------------------------------------------------------------------------------

void GLWindow::keyPressEvent(QKeyEvent *_event)
{
    // this method is called every time the main window recives a key event.
    // we then switch on the key value and set the camera in the GLWindow
    m_keysPressed += (Qt::Key)_event->key();

    switch (_event->key())
    {
    case Qt::Key_W : toggleWireframe(); break;

    case Qt::Key_R : reset(); break;

    case Qt::Key_N : toggleNormals(); break;

    case Qt::Key_1 : m_RM->enablePhongShading(); break;

    case Qt::Key_2 : m_RM->enableSurfaceShading(); break;

    case Qt::Key_Space :spacebarEvent(); break;

    case Qt::Key_Return : enterEvent(); break;

    case Qt::Key_Tab : switchFollowTarget(); break;

    case Qt::Key_H : toggleHelp(); break;



    default : break;
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::keyReleaseEvent(QKeyEvent *_event)
{
    m_keysPressed -= (Qt::Key)_event->key();

}
//----------------------------------------------------------------------------------------------------------------------

void GLWindow::moveBundle()
{
    float increment = 0.005;
    float xDirection = 0.0;
    float yDirection = 0.0;
    foreach(Qt::Key key, m_keysPressed)
    {
        switch (key)
        {
            case Qt::Key_Left :  { xDirection-=increment; break;}
            case Qt::Key_Right : { xDirection+=increment; break;}
            case Qt::Key_Up :		 { yDirection+=increment; break;}
            case Qt::Key_Down :  { yDirection-=increment; break;}
            default : break;
        }
    }

    Bundle *b = m_myCam->getFollowTarget();
    ngl::Vector f_move = ngl::Vector(xDirection, yDirection, 0.0);
    if(m_keysPressed.size() != 0)
    {
        b->setAcc(m_myCam->getViewMatrix()*f_move); //move relative to the screen
    }else{
        b->setAcc(ngl::Vector(0.0, 0.0, 0.0));
    }
}



//----------------------------------------------------------------------------------------------------------------------

void GLWindow::spacebarEvent()
{
//    PAUSED = true;
    Bundle *b =  m_myCam->getFollowTarget();
    m_RM->createRibbon(b);
//    PAUSED = false;
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::enterEvent()
{
    /* Split the bundle into two when you hit enter */

    Bundle *currentTarget = m_myCam->getFollowTarget();

    if(currentTarget->getRibbons()->size() > 1)
    {
        Bundle *db = m_RM->createDirectedBundle(currentTarget->getPos(),m_RM->createRandVector(0));

        Bundle *db2 = m_RM->createDirectedBundle(currentTarget->getPos(),m_RM->createRandVector(1));

        std::vector<Bundle*> out;
        out.push_back(db);
        out.push_back(db2);

        m_RM->splitBundle(currentTarget,out);
        m_myCam->follow(db);
        m_RM->deleteBundle(currentTarget);
    }
}
//----------------------------------------------------------------------------------------------------------------------
void GLWindow::switchFollowTarget()
{
    std::vector<Bundle*> *bundles = m_RM->getBundles();

    for(std::vector<Bundle*>::iterator it=bundles->begin(); it<bundles->end(); it++)
    {
        if(*it == m_myCam->getFollowTarget())
        {
            if(it < bundles->end()-1)
            {
                m_myCam->follow(*(it+1));
                break;
            }
            else
            {
                m_myCam->follow(*(bundles->begin()));
                break;
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------

void GLWindow::timerEvent(
                          QTimerEvent *_event
                         )
{
// if the timer is the light timer call the update light method
  if(_event->timerId() == m_timer)
  {
      incrementTime();
      moveBundle();
  }
    // re-draw GL
updateGL();
}



//----------------------------------------------------------------------------------------------------------------------

void GLWindow::reset()
{
    delete m_RM;
    delete m_myCam;
    delete m_light;

    initializeGL();

    std::cout<<"Reset"<<std::endl;

}
