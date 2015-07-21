#include "BranchBundle.h"


BranchBundle::BranchBundle(const int _id,
                           const ngl::Vector &_startPos,
                           const ngl::Vector &_endPos,
                           const float &_startFrame,
                           const float &_frames, RibbonManager *_parent) : Bundle(_id,_parent)
{
    m_type = BRANCH;

    m_startPos = _startPos;
    m_endPos = _endPos;
    m_startFrame = _startFrame;
    m_frames = _frames;

    m_pos = m_startPos;

    m_springStrength = 0.9;

    ACTIVE=false;
}
//----------------------------------------------------------------------------------------------------------------------
void BranchBundle::update()
{
    if(m_parent->getTime() == m_startFrame) ACTIVE=true;
    if(m_parent->getTime() == m_startFrame+m_frames) ACTIVE=false;

    if(ACTIVE)
    {
        float t = (m_parent->getTime()-m_startFrame)/m_frames;
        ngl::Vector tmp_oldPos = m_pos;
        m_pos = ngl::lerp(m_startPos, m_endPos, t);

        m_twist = 5;
        ngl::Vector V =m_pos-tmp_oldPos;;
        if(V==0){
            V = ngl::Vector(1,0,0,0);
        }
        m_rotQuat.fromAxisAngle(V,m_twist);

    }


    for(std::vector<Ribbon*>::iterator it=m_ribbons.begin(); it<m_ribbons.end(); it++)
    {
        (*it)->update();
    }
}
//----------------------------------------------------------------------------------------------------------------------
