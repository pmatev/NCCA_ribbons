#include "Solo.h"

Solo::Solo(int _id, RibbonManager *_parent) : Bundle(_id, _parent)
{
    m_type = SOLO;

    m_springStrength = 2.0;

    m_pos = ngl::Vector(0,0,0);
    m_vel = ngl::Vector(0,0,0.1);
    m_acc = ngl::Vector(0,0,0);

    m_twist = 5;
}
//----------------------------------------------------------------------------------------------------------------------
void Solo::update()
{

        ngl::Vector tmp_oldPos = m_pos;

        m_vel += m_acc;
        m_vel *= 0.95;
        m_vel.m_z = 0.1;
        m_pos += m_vel;

    ngl::Vector V =m_pos-tmp_oldPos;;
    if(V==0){
        V = ngl::Vector(1,0,0,0);
    }
    m_rotQuat.fromAxisAngle(V,m_twist);

    for(std::vector<Ribbon*>::iterator it=m_ribbons.begin(); it<m_ribbons.end(); it++)
    {
        (*it)->update();
    }
}

//----------------------------------------------------------------------------------------------------------------------

