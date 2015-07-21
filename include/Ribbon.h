#ifndef RIBBON_H
#define RIBBON_H


#include <vector>
#include "ngl/TransformStack.h"
#include "ngl/Vector.h"
#include "ngl/VertexArrayObject.h"
#include "Axis.h"
#include "Bundle.h"

class Bundle;
class GLWindow;
class VertexArrayObject;

/// @file Ribbon.h
/// @brief A class to create, update and draw a Ribbon.
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/2012
/// Revision History :
/// Initial Version 29/03/2012
/// @class Ribbon
/// @brief Creates, updates and draws a Ribbon of a specified length and width. Takes in a colour value and has the ability
/// to switch between different shading models. Contains a local Axis for debugging perposes.
/// Also calculates a twist based on quaternion rotations.
class Ribbon
{

public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief enum to define the different types of shading available
    //----------------------------------------------------------------------------------------------------------------------
    enum SHADING{PHONG, SURFACE};

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Ribbon Constructor
    /// @param [in] _id unique id
    /// @param [in] _c Colour for the ribbon
    /// @param [in] _length length of ribbon
    /// @param [in] _width width of ribbon
    /// @param [in] _parent parent Bundle
    /// @param [in] _gl GLWindow context
    //----------------------------------------------------------------------------------------------------------------------
    Ribbon(const int _id, const ngl::Colour _c, const int _length,const float _width, Bundle *_parent, GLWindow *_gl);
    ~Ribbon();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get current position
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector getPos(){return m_pos;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get current velocity
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector getVel(){return m_vel;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get current acceleration
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector getAcc(){return m_acc;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get local transformation
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::TransformStack getLocalTransform(){return m_tx;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get ribbon id
    //----------------------------------------------------------------------------------------------------------------------
    inline int getID(){return m_id;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get vbo id /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    inline GLuint getVBOID(){return m_vboID;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief return ribbon's shading type
    //----------------------------------------------------------------------------------------------------------------------
    inline int getShadingType(){return m_shadingType;}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get pointer to the position history list
    //----------------------------------------------------------------------------------------------------------------------
    inline std::vector<ngl::Vector>* getPosHistory(){return &m_posHistory;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set parent bundle
    /// @param [in] _p the bundle to assign as parent
    //----------------------------------------------------------------------------------------------------------------------
    inline void setParent(Bundle *_p){m_parent = _p;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set shading type
    /// @param [in] _i the type of shading e.g. PHONG, SURFACE
    //----------------------------------------------------------------------------------------------------------------------
    inline void setShadingType(const int &_i){m_shadingType = _i;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update the position of the tip of the ribbon
    //----------------------------------------------------------------------------------------------------------------------
    void update();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief work out vertices and normals then draw
    //----------------------------------------------------------------------------------------------------------------------
    void draw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief draw the local axis (for debugging purposes)
    //----------------------------------------------------------------------------------------------------------------------
    void drawLocalAxis();

protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the GLWindow
    //----------------------------------------------------------------------------------------------------------------------
    GLWindow *m_gl;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief parent Bundle
    //----------------------------------------------------------------------------------------------------------------------
    Bundle *m_parent;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief local axis
    //----------------------------------------------------------------------------------------------------------------------
    Axis *m_localAxis;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief time value passed down from GLWindow
    //----------------------------------------------------------------------------------------------------------------------
    float m_time;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief unique id
    //----------------------------------------------------------------------------------------------------------------------
    int m_id;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief shading colour
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Colour m_col;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief length of PAIRS of verts e.g. m_length 10 = 20 edgeVerts = 60 triangles
    //----------------------------------------------------------------------------------------------------------------------
    int m_length;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief width of ribbon
    //----------------------------------------------------------------------------------------------------------------------
    float m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief local twisting value
    //----------------------------------------------------------------------------------------------------------------------
    float m_twist;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief velocity of wind speed /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    float m_wind;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief position
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_pos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief velocity
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_vel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief acceleration
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_acc;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief local transformation stack
    //----------------------------------------------------------------------------------------------------------------------
    ngl::TransformStack m_tx;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector of all previous histories up to m_length
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vector> m_posHistory;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector of all edge vertices i.e. history of v0 and v1
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vec3> m_edgeVertHistory;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector of all opengl triangle verts with correct winding
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vec3> m_vertHistory;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector of all per-vertex normals
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ngl::Vec3> m_normHistory;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the Vertex Array Object for drawing the verts
    //----------------------------------------------------------------------------------------------------------------------
    ngl::VertexArrayObject *m_vao;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief VBO id /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    GLuint m_vboID;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief size of VBO on previous frame /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    int m_oldVBOSize;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load the transform stack to the surface shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToSurfaceShader(ngl::TransformStack &_tx);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load the transform stack to the Phong shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToPhongShader(ngl::TransformStack &_tx);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief load the transform stack to the Normal shader
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToNormalShader(ngl::TransformStack &_tx);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief calculate the new tip vertices based on forces and twist
    //----------------------------------------------------------------------------------------------------------------------
    void calcVertices();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief build the Vertex Array Object by winding the verts and normals of
    /// the entire length of the ribbon and smooth normals
    //----------------------------------------------------------------------------------------------------------------------
    void buildVAO();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief variable to store shading type
    //----------------------------------------------------------------------------------------------------------------------
    int m_shadingType;
};

#endif // RIBBON_H
