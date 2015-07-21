#include "include/DirectedBundle.h"

DirectedBundle::DirectedBundle(const int &_id,
                               const ngl::Vector &_startPos,
                               const ngl::Vector &_vel,
                               RibbonManager *_parent)  : Bundle(_id, _parent)

{
    m_type = DIRECTED;
    m_pos = _startPos;
    m_vel = _vel;
    m_acc = ngl::Vector(0,0,0);
    m_springStrength = 1;

    m_twist = 5;
}
//----------------------------------------------------------------------------------------------------------------------
void DirectedBundle::update()
{

    ngl::Vector tmp_oldPos = m_pos;

    m_vel += m_acc;

    if(m_vel.lengthSquared() > 0.5)
    {
        m_vel.normalize(); //max velocity
        m_vel *= 0.5;
    }

    m_vel *= 0.99;
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
