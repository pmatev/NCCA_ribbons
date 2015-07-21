#include "Axis.h"
#include "ngl/Material.h"
#include "ngl/Camera.h"
#include <QDebug>

//----------------------------------------------------------------------------------------------------------------------
Axis::Axis(
           const float &_scale,
        Camera *_cam
          )
{
  //m_shaderName=_shaderName;
  m_scale=_scale;
  m_tx = ngl::TransformStack();

  m_cam = _cam;
  //qDebug()<<"Shader name is "<<_shaderName.c_str();

  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  prim->createCylinder("nglAXISCylinder",0.02,2,5,8);
  prim->createCone("nglAXISCone",0.05,0.2,5,5);
}
//----------------------------------------------------------------------------------------------------------------------
Axis::Axis(const ngl::Vector &_x, const ngl::Vector &_y, const ngl::Vector &_z, const float &_scale)
{
    m_vecX = _x;
    m_vecY = _y;
    m_vecZ = _z;
    m_scale = _scale;

    m_tx = ngl::TransformStack();
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    prim->createCylinder("nglAXISCylinder",0.02,2,5,8);
    prim->createCone("nglAXISCone",0.05,0.2,5,5);
}
//----------------------------------------------------------------------------------------------------------------------
Axis::~Axis()
{
   // std::cout<<"Axis dtor"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
void Axis::loadMatricesToShader(ngl::TransformStack &_tx)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["Surface"]->use();
  ngl::Matrix MV;
  ngl::Matrix MVP;
  ngl::Matrix M;
  M=_tx.getCurrentTransform().getMatrix();
  MV=  _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
  MVP=  MV*m_cam->getProjectionMatrix();
  shader->setShaderParamFromMatrix("MVP",MVP);
}




//----------------------------------------------------------------------------------------------------------------------
void Axis::draw()
{
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  // grab an instance of the shader manager
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  (*shader)["Surface"]->use();
  // Rotation based on the mouse position for our global
  // transform
   // set this in the TX stack
  // liberal use of { is encourage after each push

  ngl::TransformStack txStack;
  ngl::Transformation tx;
  txStack.pushTransform();
  {

    // set the global rotation
    txStack.setGlobal(m_tx.getGlobalTransform());

    txStack.pushTransform();
    {
   // liberal use of { is encourage after each push

        /* **** X Axis **** */
        shader->setShaderParam4f("Colour",1,0,0,1);

        txStack.pushTransform();
        {
            txStack.setScale(m_scale,m_scale,m_scale);

//            ngl::Vector world_x_axis = ngl::Vector(1,0,0,0);
//            float angleX = m_vecX.angleBetween(world_x_axis);

            txStack.setRotation(0,-90,0);
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCylinder");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.setScale(m_scale,m_scale,m_scale);
            txStack.setPosition(ngl::Vector(m_scale,0,0));
            txStack.setRotation(0,90,0);
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.setScale(m_scale,m_scale,m_scale);
            txStack.setPosition(ngl::Vector(-m_scale,0,0));
            txStack.setRotation(0,-90,0);
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();

        /* ******* Y Axis ****** */
        shader->setShaderParam4f("Colour",0,1,0,1);

        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            txStack.getCurrentTransform().setRotation(90,0,0);
            txStack.getCurrentTransform().setPosition(ngl::Vector(0,0,0));
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCylinder");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            txStack.getCurrentTransform().setRotation(-90,0,0);
            txStack.getCurrentTransform().setPosition(ngl::Vector(0,m_scale,0));
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            txStack.getCurrentTransform().setRotation(90,0,0);
            txStack.getCurrentTransform().setPosition(ngl::Vector(0,-m_scale,0));
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();

        /* **** Z Axis **** */
        shader->setShaderParam4f("Colour",0,0,1,1);

        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            //txStack.getCurrentTransform().setPosition(ngl::Vector(0,0,m_scale));
            txStack.getCurrentTransform().setRotation(0,180,0);
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCylinder");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            txStack.getCurrentTransform().setPosition(ngl::Vector(0,0,m_scale));
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();
        txStack.pushTransform();
        {
            txStack.getCurrentTransform().setScale(m_scale,m_scale,m_scale);
            txStack.getCurrentTransform().setPosition(ngl::Vector(0,0,-m_scale));
            txStack.getCurrentTransform().setRotation(180,0,0);
            loadMatricesToShader(txStack);
            prim->draw("nglAXISCone");
        }
        txStack.popTransform();


  }
  txStack.popTransform();
}
txStack.popTransform();


}
//----------------------------------------------------------------------------------------------------------------------
