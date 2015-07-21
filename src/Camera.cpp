#include <iostream>
#include "ngl/Camera.h"
#include "ngl/Quaternion.h"
#include "Camera.h"
#include "GLWindow.h"

Camera::Camera()
{
    std::cout<<"Camera created"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
Camera::Camera(const ngl::Vector &_eye, const ngl::Vector &_look, const ngl::Vector &_up, GLWindow *_parent)
{
    //ngl::Camera.m_projectionMode = ngl::PERSPECTIVE;
    this->setDefaultCamera();
    //this->set(_eye,_look,_up);

    m_eye = _eye;
    m_look = _look;
    m_up = _up;

    m_parent = _parent;

    m_rotAxis = ngl::Vector(0,1,0,0);
    m_radius=1.5;

    FOLLOW=false;
}
//----------------------------------------------------------------------------------------------------------------------
Camera::~Camera()
{
    std::cout<<"dtor Camera"<<std::endl;
}

void Camera::update()
{
    if(FOLLOW)
    {
        m_eye = m_target->getPos()+(m_eye-m_look);
        m_look = m_target->getPos();
    }
    setNew();
}

//----------------------------------------------------------------------------------------------------------------------
void Camera::setNew()
{
    this->set(m_eye, m_look, ngl::Vector(0,1,0,0));
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::dolly(int _oldX, int _newX)
{
    update();
    int dx =  _oldX - _newX;
    //m_radius = dx*0.01;

    ngl::Vector N = getN();

    m_eye += N*(dx*0.1);

    setNew();
    //std::cout<<"dolly"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::track(int _oldX, int _oldY, int _newX, int _newY)
{
    update();
    int dx = _newX - _oldX;
    int dy = _newY - _oldY;
    dx*=-1;

    ngl::Vector v(dx, dy, 0);
    v.normalize();
    v = getViewMatrix()*v;

    v *= 0.1;

    m_eye += v;
    m_look += v;



    setNew();

    //std::cout<<"track"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
void Camera::tumble(int _oldX, int _oldY, int _newX, int _newY)
{
    /* Ensure mouse has moved */
    if(_oldX == _newX && _oldY==_newY)
    {
        return;
    }

    update();


    /* Map screen coords of v1 and v2 to sphere in camera space */

    ngl::Matrix V = this->getViewMatrix();

    double oldX = 2*(double) _oldX / (double) m_parent->getScreenWidth();
    double oldY = 2*(double) _oldY / (double) m_parent->getScreenHeight();

    oldX = oldX - 1;
    oldY = 1 - oldY;

    double oldZ = std::sqrt(m_radius*m_radius - oldX*oldX - oldY*oldY);

    ngl::Vector cam_v1(oldX, oldY, oldZ);
    //std::cout<<v1<<std::endl;


    double newX = 2*(double) _newX / (double) m_parent->getScreenWidth();
    double newY = 2*(double) _newY / (double) m_parent->getScreenHeight();

    newX = newX - 1;
    newY = 1 - newY;


    double newZ = std::sqrt(m_radius*m_radius - newX*newX - newY*newY);

    ngl::Vector cam_v2(newX, newY, newZ);

    /* map to world space */

    ngl::Vector wrld_v1 = V*cam_v1;
    ngl::Vector wrld_v2 = V*cam_v2;


    // get rotation axis by v1 x v2
    m_rotAxis = wrld_v1.cross(wrld_v2);
    // get angle from v1 dot v2
    float theta = wrld_v1.angleBetween(wrld_v2);

    /* quaternion rotation from axis angle */
    ngl::Quaternion q, q_I, q_cam;
    q.fromAxisAngle(m_rotAxis,theta*300);
    q_I = q.inverse();

    ngl::Vector camEye = m_eye - m_look;                        // move back to origin to rotate
    q_cam = ngl::Quaternion(0,camEye[0],camEye[1],camEye[2]);   // make camEye quaternion

    ngl::Quaternion q_newCamEye = q_I*q_cam*q;                  // rotate camEye by rotation quaternion
    ngl::Vector newCamEye = q_newCamEye.getVector();
    //ngl::Vector newCamEye = rotQuat * camEye;

    //ngl::Vector d_Eye = newCamEye - camEye;
    //this->moveEye(d_Eye[0],d_Eye[1],d_Eye[2]);

    m_eye = newCamEye + m_look;                                 //move back to look pos

    setNew();

}
