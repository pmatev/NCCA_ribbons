#include "RibbonManager.h"
#include "iostream"
#include "Solo.h"
#include "BranchBundle.h"
#include "DirectedBundle.h"

RibbonManager::RibbonManager(Camera *_c, GLWindow *_p)
{
    m_cam = _c;
    m_parent = _p;
    m_lastBundleID = 0;
    m_lastRibbonID = 0;
}
//----------------------------------------------------------------------------------------------------------------------
RibbonManager::~RibbonManager()
{
    for(std::vector<Bundle*>::iterator it=m_bundles.begin(); it< m_bundles.end(); it++)
    {
        delete (*it);
    }
    std::cout<<"Ribbon Manager dtor"<<std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
//Bundle* RibbonManager::createBranch(const ngl::Vector &_s, const ngl::Vector &_e, const float &_startFrame, const float &_frames)
//{
//    Bundle *b = new BranchBundle(_s, _e, _startFrame, _frames, this);
//    m_bundles.push_back(b);
//    return b;
//}
Bundle* RibbonManager::createBranch(const int &_id, const ngl::Vector &_s, const ngl::Vector &_e, const float &_startFrame, const float &_frames)
{
    Bundle *b = new BranchBundle(_id, _s, _e, _startFrame, _frames, this);
    m_bundles.push_back(b);
    m_lastBundleID += 1;

    return b;
}

Bundle* RibbonManager::createDirectedBundle(const ngl::Vector &_pos, const ngl::Vector _vel)
{
    Bundle *b = new DirectedBundle(m_lastBundleID+1, _pos, _vel, this);
    m_bundles.push_back(b);
    m_lastBundleID += 1;
    return b;
}

Bundle* RibbonManager::createSolo()
{
    Bundle *b = new Solo(0,this);
    m_bundles.push_back(b);
    m_lastBundleID = 0;
    return b;
}

void RibbonManager::createRandomRibbons(const int &_numRibbons, Bundle *_bundle)
{
    for (int i=0; i<_numRibbons; i++)
    {
        ngl::Colour rand = createRandColour();
        _bundle->getRibbons()->push_back(new Ribbon(i,rand,500,0.2,_bundle,getParent()));
        m_lastRibbonID = i;
    }
}

ngl::Colour RibbonManager::createRandColour()
{
    srand(time(0));
    float r = rand()%10 /10.0 ;
    float g = rand()%10 /10.0;
    float b = rand()%10 /10.0;

    return ngl::Colour(r, g, b);
}
ngl::Vector RibbonManager::createRandVector(int _seed)
{
    srand(time(0)+_seed);
    float x = 1- (rand()%10 /5.0);
    float y = 1- (rand()%10 /5.0);
    float z = 1- (rand()%10 /5.0);

    return ngl::Vector(x, y, z);
}


void RibbonManager::createRibbon(Bundle *_bundle)
{
    Ribbon *r = new Ribbon(m_lastRibbonID+1, createRandColour(), 1000,0.5, _bundle, m_parent);
    _bundle->addRibbon(r);

    m_lastRibbonID += 1;
}

int RibbonManager::getPosInList(Bundle *_b)
{
    int size = m_bundles.size();
    for(int i=0; i<size; i++)
    {
        if(m_bundles.at(i) == _b)
        {
            return i;
        }
    }
}

void RibbonManager::deleteBundle(Bundle *_b)
{
    int pos = getPosInList(_b);
    m_bundles.erase(m_bundles.begin()+pos);
    delete _b;
}

void RibbonManager::splitBundle(Bundle *_in, std::vector<Bundle *> _outBundles)
{
    std::vector<Ribbon*> *ribbons = _in->getRibbons();
    const int splitCount = _outBundles.size();
    for(int r=0; r<ribbons->size(); r++)
    {
        for(int b=0; b<splitCount; b++)
        {
            if(r%splitCount == b)
            {
                //std::cout<<r<<" "<<b<<std::endl;
                _outBundles.at(b)->addRibbon(ribbons->at(r));
                ribbons->at(r)->setParent(_outBundles.at(b));
            }
        }
    }
   ribbons->clear();
//    deleteBundle(_in);
}


void RibbonManager::enablePhongShading()
{
    for(std::vector<Bundle*>::iterator it=m_bundles.begin(); it<m_bundles.end(); it++)
    {
        (*it)->setShadingType(Ribbon::PHONG);
    }
}

void RibbonManager::enableSurfaceShading()
{
    for(std::vector<Bundle*>::iterator it=m_bundles.begin(); it<m_bundles.end(); it++)
    {
        (*it)->setShadingType(Ribbon::SURFACE);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void RibbonManager::updateBundles()
{
    for(std::vector<Bundle*>::iterator it=m_bundles.begin(); it<m_bundles.end(); it++)
    {
        /*if((*it)->getRibbons()->size()==0)
        {
            m_bundles.erase(it);
            delete (*it);
        }
        else */(*it)->update();
    }
}
//----------------------------------------------------------------------------------------------------------------------
void RibbonManager::drawBundles()
{
    for(std::vector<Bundle*>::iterator it=m_bundles.begin(); it<m_bundles.end(); it++)
    {
//        (*it)->drawAxis();
        (*it)->draw();
    }
}
//----------------------------------------------------------------------------------------------------------------------
int RibbonManager::getTime()
{
    return m_parent->getTime();
}
