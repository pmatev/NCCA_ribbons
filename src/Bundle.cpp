#include "Bundle.h"

Bundle::Bundle(int _id, RibbonManager *_parent)
{
    m_id = _id;
    m_parent = _parent;

    m_twist = 0;
    m_radius = 3;

    m_axis = new Axis(1.0, m_parent->getCam());

}

//----------------------------------------------------------------------------------------------------------------------
Bundle::~Bundle()
{
    if(m_ribbons.size()>0){
        for(std::vector<Ribbon*>::iterator it = m_ribbons.begin(); it< m_ribbons.end(); it++)
        {
            delete (*it);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void Bundle::draw()
{
    //std::cout<<"draw BUNDLE"<<std::endl;
    for(std::vector<Ribbon*>::iterator it = m_ribbons.begin(); it< m_ribbons.end(); it++)
    {
        //(*it)->drawLocalAxis();
        (*it)->draw();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void Bundle::drawAxis()
{
    ngl::Transformation tx;
    tx.setPosition(m_pos);

    m_axis->setGlobalTransform(tx);
    m_axis->draw();
}
//----------------------------------------------------------------------------------------------------------------------


void Bundle::setShadingType(const int &_id)
{
    for(std::vector<Ribbon*>::iterator it=m_ribbons.begin(); it< m_ribbons.end(); it++)
    {
        (*it)->setShadingType(_id);
    }
}



//----------------------------------------------------------------------------------------------------------------------

int Bundle::getTime(){return m_parent->getTime();}

//----------------------------------------------------------------------------------------------------------------------
Camera* Bundle::getCam(){return m_parent->getCam();}
//----------------------------------------------------------------------------------------------------------------------

void Bundle::createRibbon(const int &_id, const ngl::Colour &_c, const int &_l, const float &_w)
{
    Ribbon *r = new Ribbon(_id,_c,_l,_w,this,m_parent->getParent());
    m_ribbons.push_back(r);
}

//----------------------------------------------------------------------------------------------------------------------
void Bundle::addRibbon(Ribbon *_r)
{
    m_ribbons.push_back(_r);
}
//----------------------------------------------------------------------------------------------------------------------

//void Bundle::removeRibbon(const int _id)
//{
//    m_ribbons.clear();
//}

//void Bundle::addRibbons(const int &_num, std::vector<Ribbon *> *_r)
//{
//    for(std::vector<Ribbon*>::iterator it = _r->begin(); it<_r->end(); it++)
//    {
//        m_ribbons.push_back((*it));
//    }
//}
