#include "Ribbon.h"
#include "ngl/ShaderLib.h"
#include "ngl/Quaternion.h"
#include "Axis.h"
#include "ngl/Material.h"
#include "time.h"


Ribbon::Ribbon(const int _id, const ngl::Colour _c,const int _l, const float _w, Bundle *_p, GLWindow *_gl)
{
    m_id  = _id;
    m_parent = _p;
    m_gl = _gl;
    m_length = _l;
    m_width = _w;
    m_twist = 90;
    m_wind = 0.02;
    m_col = _c;

    m_shadingType = SURFACE;

    srand(m_id);
    float x = 2.0*rand()/RAND_MAX -1;
    srand(m_id+1);
    float y = 2.0*rand()/RAND_MAX - 1;
    srand(m_id+2);
    float z = 2.0*rand()/RAND_MAX - 1;
    ngl::Vector randPos = ngl::Vector(x, y, z);
    m_pos = m_parent->getPos()+ 25*randPos;


    m_cam = m_gl->getCam();

    m_localAxis = new Axis(m_width,m_cam);

    m_vao= ngl::VertexArrayObject::createVOA(GL_TRIANGLES);

//    m_vboID = m_vao->getVBOid(0);

//    m_vao->bindVBO(m_vboID);

//    int max_vboSize = m_length*6*sizeof(ngl::Vec3);
//    m_vao->setData(m_vboID,max_vboSize,0,GL_STREAM_DRAW);

//    m_vao->unbindVBO();

}
//----------------------------------------------------------------------------------------------------------------------
Ribbon::~Ribbon()
{
    //std::cout<<"Ribbon dtor"<<std::endl;
    m_vao->removeVOA();
    delete m_vao;
    delete m_localAxis;
}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::update()
{
    ngl::Vector parentPos = m_parent->getPos();
    ngl::Vector vToParent = (parentPos-m_pos);




    m_time = m_gl->getTime();
   // float distToParent = vToParent.length();


    // F = -kX
    float k = (m_parent->getSpringStrength());
    ngl::Vector f_center = k*(m_parent->getRadius()/100.0)*vToParent;
    ngl::Vector f_damping = -0.1*m_vel;

    std::vector<Ribbon*> *ribbons = m_parent->getRibbons();
    ngl::Vector f_separation(0,0,0);

    for(std::vector<Ribbon*>::iterator it = ribbons->begin(); it<ribbons->end(); it++)
    {
        /* ****  F = -1/d2  ****** */
        /* Separation */
        if((*it)->getID()!= m_id)
        {
            ngl::Vector v = m_pos - (*it)->getPos();
            float d2 = v.lengthSquared();
            v.normalize();
            f_separation += v/d2;

            /* Avoid Trails */
            std::vector<ngl::Vector>* i_PosHistory = (*it)->getPosHistory();

            for(int i =0; i< i_PosHistory->size(); i+=2)
            {
                ngl::Vector v = m_pos - i_PosHistory->at(i);
                float d2 = v.lengthSquared();
                v.normalize();
                f_separation += 0.01*v/d2;
            }
        }

    }


//    float k_vortexStrength = m_parent->getTwist();

//    ngl::Vector tangU = vToParent.cross(ngl::Vector(0,0,1));
//    tangU.normalize();
//    ngl::Vector tangV = tangU.cross(vToParent);
//    tangV.normalize();
//    ngl::Vector f_vortex = tangV;


    /* rotation based on parent */
    ngl::Quaternion q_Parent = m_parent->getRotQuat();
    ngl::Vector tmpPos = m_pos - parentPos;
    q_Parent.rotatePoint(q_Parent, tmpPos);
    m_pos = tmpPos + parentPos;


    m_acc = f_center + f_damping + f_separation*0.3/* + 2*f_vortex*/;

//    if(m_acc.lengthSquared() > 0.05)
//    {
//        m_acc.normalize();
//        m_acc *= 0.05;
//    }

//    if(m_acc.lengthSquared() > 1.0)m_acc.normalize();
//    else if(m_acc.lengthSquared() <0)m_acc *= 0.01;

    m_vel += m_acc;
//    if(m_vel.lengthSquared() > 0.5)
//    {
//        m_vel.normalize(); //max velocity
//        m_vel *= 0.5;
//    }
    m_pos += m_vel;//+ m_parent->getVel();

    m_twist += 1;


    /* Set transform stack */
    //m_tx.setPosition(-m_pos);
    m_tx.pushTransform();
        m_tx.setPosition(m_pos);
    m_tx.popTransform();

    /* Add to history */
    m_posHistory.push_back(m_pos);
    if( m_posHistory.size() > m_length ) m_posHistory.erase(m_posHistory.begin());

}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::calcVertices()
{




        /* local twist */
        ngl::Vector p0 = m_pos;
        ngl::Vector p1, V;

        /* Calc rotation quaternion if there is more than 1 line segment */
        if(m_posHistory.size() > 1)
        {
            p1 = m_posHistory.at(m_posHistory.size()-2);
            V = p0 - p1;
            //V.m_w = 0.0;
        }else{
            V = ngl::Vector(0,0,1,0);
        }
        V.normalize();
        ngl::Vector U;
        if(V != ngl::Vector(0,1,0))
        {
            U = V.cross(ngl::Vector(0,1,0));
        }else{
            U = V.cross(ngl::Vector(1,0,0));
        }
        U.normalize();
        ngl::Vector N = V.cross(U);
        N.normalize();

//        m_localAxis->set_x_axis(V);
//        m_localAxis->set_y_axis(U);
//        m_localAxis->set_z_axis(N);

        ngl::Vector v_v0(0,0,0);
        ngl::Vector v_v1(0,0,0);

        /* vertices centered at origin */
        v_v0 += (N*m_width/2);
        v_v1 += (-N*m_width/2);

        ngl::Quaternion q;
        q.fromAxisAngle(V,m_twist);

        q.rotatePoint(q, v_v0);
        q.rotatePoint(q, v_v1);

        ngl::Vec3 new_v0 = (v_v0+m_pos).toVec3();
        ngl::Vec3 new_v1 = (v_v1+m_pos).toVec3();

        m_edgeVertHistory.push_back(new_v0);
        m_edgeVertHistory.push_back(new_v1);
        if( m_edgeVertHistory.size() > m_length*2 )
        {
            m_edgeVertHistory.erase(m_edgeVertHistory.begin());
            m_edgeVertHistory.erase(m_edgeVertHistory.begin());
        }

}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::buildVAO()
{
    m_vertHistory.clear();
    m_normHistory.clear();


    for(int i=0; i<m_length*2; i+=2)
    {
        ngl::Vec3 V0,V1,V2,V3,V4,V5;
        ngl::Vec3 n0,n1,n2, n3;

        if(m_edgeVertHistory.size()>i+3)
        {
            V0 = m_edgeVertHistory[i];
            V1 = m_edgeVertHistory[i+1];
            V2 = m_edgeVertHistory[i+2];
            V3 = m_edgeVertHistory[i+3];

            /* vertices */
            m_vertHistory.push_back(V0);
            m_vertHistory.push_back(V1);
            m_vertHistory.push_back(V2);
            m_vertHistory.push_back(V1);
            m_vertHistory.push_back(V3);
            m_vertHistory.push_back(V2);

            /* Hard Faces */

            /* normals */
            n0 = ngl::calcNormal(V0,V1,V2);
//            m_normHistory.push_back(n0);
//            m_normHistory.push_back(n0);
//            m_normHistory.push_back(n0);

            n1 = ngl::calcNormal(V1, V3, V2);
//            m_normHistory.push_back(n1);
//            m_normHistory.push_back(n1);
//            m_normHistory.push_back(n1);
        }



        /* Smoothed Normals */
        if(m_edgeVertHistory.size()>i+5)
        {
            V4 = m_edgeVertHistory[i+4];
            V5 = m_edgeVertHistory[i+5];

            n2 = ngl::calcNormal(V2,V3, V4);
            n3 = ngl::calcNormal(V3,V5,V4);

            ngl::Vec3 smooth_n2 = n0+n2;
            smooth_n2.normalize();

            ngl::Vec3 smooth_n3= n1+n3;
            smooth_n3.normalize();

            m_normHistory.push_back(n0);
            m_normHistory.push_back(n1);
            m_normHistory.push_back(smooth_n2);
            m_normHistory.push_back(n1);
            m_normHistory.push_back(smooth_n3);
            m_normHistory.push_back(smooth_n2);
        }
        else if(m_edgeVertHistory.size()>i+3)
        {
            m_normHistory.push_back(n0);
            m_normHistory.push_back(n0);
            m_normHistory.push_back(n0);
            m_normHistory.push_back(n1);
            m_normHistory.push_back(n1);
            m_normHistory.push_back(n1);
        }
    }




    if(m_edgeVertHistory.size()>m_length*2)
    {
//        m_edgeVertHistory.erase(m_edgeVertHistory.begin(),m_edgeVertHistory.begin()+1);
        m_edgeVertHistory.erase(m_edgeVertHistory.begin());
        m_edgeVertHistory.erase(m_edgeVertHistory.begin());
    }


    /* check that each vertex has a normal */
    if(m_vertHistory.size() != m_normHistory.size())
    {
        std::cout<<"ERROR: VertHistory: "<<m_vertHistory.size()<<" NormHistory: "<<m_normHistory.size()<<std::endl;
    }




    int sizeVec3 = sizeof(ngl::Vec3);
    int t_sizeof = m_vertHistory.size()*sizeVec3;



//    m_vao->bindVBO(m_vboID);

//    m_vao->setSubData(0,t_sizeof,m_vertHistory[0].m_x);
//    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);


//    int vboSize = m_vao->getVBOsize();

//    m_vao->setData(m_vboID, t_sizeof,m_normHistory[0].m_x,GL_STREAM_DRAW);
//    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

//    m_oldVBOSize = m_vao->getVBOsize();
//    m_vao->unbindVBO();


    m_vao->bind();
    //verts
    m_vao->setData(t_sizeof,m_vertHistory[0].m_x,GL_STREAM_DRAW);
    m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);

    //normals
    m_vao->setData(t_sizeof,m_normHistory[0].m_x,GL_STREAM_DRAW);
    m_vao->setVertexAttributePointer(2,3,GL_FLOAT,0,0);

    m_vao->setNumIndices((int) m_vertHistory.size());

    m_vao->unbind();


}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::draw()
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();


    calcVertices();
    buildVAO();



    /* ***** Draw Normals ***** */
    if(m_gl->getNORMALS())
    {
        (*shader)["normalShader"]->use();

        loadMatricesToNormalShader(m_tx);

        m_vao->bind();
        m_vao->draw();
        m_vao->unbind();
    }
    /* ***** Draw Ribbon Material ****** */
    if(m_shadingType == SURFACE)
    {
        (*shader)["Surface"]->use();
        loadMatricesToSurfaceShader(m_tx);
        shader->setShaderParam4f("Colour",m_col.r(),m_col.g(),m_col.b(),1);
    }
    else if(m_shadingType == PHONG)
    {
        (*shader)["Phong"]->use();
        loadMatricesToPhongShader(m_tx);

        ngl::Material mFront(ngl::Colour(0,0,0),m_col,ngl::Colour(0.3,0.3,0.3),1.0f);
        mFront.setSpecularExponent(5.3f);
        mFront.loadToShader("material");
    }

//    ngl::Material mBack(ngl::Colour(0,0,0),ngl::Colour(m_col.r()*0.5,m_col.b()*0.5,m_col.g()*0.5),ngl::Colour(0.3,0.3,0.3),1.0f);
//    mBack.setSpecularExponent(5.3f);
    //mBack.loadToShader("backMaterial");

    m_vao->bind();
    m_vao->draw();
    m_vao->unbind();

    m_vao->removeVOA();
    //m_vao->createVOA(GL_TRIANGLES);

}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::drawLocalAxis()
{
    ngl::Transformation tipTX;
    tipTX.setPosition(m_pos);

    m_localAxis->setGlobalTransform(tipTX);
    m_localAxis->draw();
}
void Ribbon::loadMatricesToSurfaceShader(ngl::TransformStack &_tx)
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
void Ribbon::loadMatricesToPhongShader(ngl::TransformStack &_tx)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    (*shader)["Phong"]->use();
    //shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

    ngl::Matrix MV;
    ngl::Matrix MVP;
    ngl::Mat3x3 normalMatrix;
    ngl::Matrix M;
    M=_tx.getCurrentTransform().getMatrix();
    MV=  _tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
    MVP=  MV*m_cam->getProjectionMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMatrix("MV",MV);
    shader->setShaderParamFromMatrix("MVP",MVP);
    shader->setShaderParamFromMat3x3("normalMatrix",normalMatrix);
    //shader->setShaderParamFromMatrix("M",M);
}
//----------------------------------------------------------------------------------------------------------------------
void Ribbon::loadMatricesToNormalShader(ngl::TransformStack &_tx)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["normalShader"]->use();

  ngl::Matrix MV;
  ngl::Matrix MVP;

  //shader->setShaderParam3f("viewerPos",m_parent->getCam()->getEye().m_x,m_parent->getCam()->getEye().m_y,m_parent->getCam()->getEye().m_z);

  MV=_tx.getCurrAndGlobal().getMatrix()*m_cam->getViewMatrix();
  MVP=MV*m_cam->getProjectionMatrix();
  shader->setShaderParamFromMatrix("MVP",MVP);

}
