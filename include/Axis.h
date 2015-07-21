#ifndef __AXIS_H__
#define __AXIS_H__


#include "ngl/Camera.h"
#include "ngl/ShaderLib.h"
#include "ngl/TransformStack.h"
#include "ngl/VAOPrimitives.h"
#include "ngl/Vector.h"
#include "Camera.h"

class Camera;

/// @file Axis.h
/// @brief Simple class to contain and draw an axis based on Jon Macey's Axis class
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/12
/// Revision History :
/// Initial Version 17/03/12
/// @class Axis
/// @brief Simple Axis Drawing
class Axis
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for BranchBundle
    /// @param [in] _scale size of Axes
    /// @param [in] _cam the Camera
    //----------------------------------------------------------------------------------------------------------------------
    Axis(const float &_scale, Camera *_cam);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for BranchBundle
    /// @param [in] _x X-Axis direction vector
    /// @param [in] _y Y-Axis direction vector
    /// @param [in] _z Z-Axis direction vector
    /// @param [in] _scale size of Axes
    //----------------------------------------------------------------------------------------------------------------------
    Axis(const ngl::Vector &_x, const ngl::Vector &_y, const ngl::Vector &_z, const float &_scale);
    /// @brief dtor
    ~Axis();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called to draw the Axis
    //----------------------------------------------------------------------------------------------------------------------
    void draw();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief upload the tranformation matrix to the shader
    /// @param[in] _tx TransformStack to load
    //----------------------------------------------------------------------------------------------------------------------
    void loadMatricesToShader(ngl::TransformStack &_tx);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the Camera
    /// @param[in] _cam pointer to the camera to set
    //----------------------------------------------------------------------------------------------------------------------
    inline void setCam(Camera *_cam){m_cam = _cam;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the global transformation
    /// @param[in] _x
    //----------------------------------------------------------------------------------------------------------------------
    inline void setGlobalTransform(ngl::Transformation &_x){m_tx.setGlobal(_x);}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the X axis
    /// @param[in] _v normalized direction vector
    //----------------------------------------------------------------------------------------------------------------------
    inline void set_x_axis(ngl::Vector _v){m_vecX = _v;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the Y axis
    /// @param[in] _v normalized direction vector
    //----------------------------------------------------------------------------------------------------------------------
    inline void set_y_axis(ngl::Vector _v){m_vecY = _v;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the Z axis
    /// @param[in] _v normalized direction vector
    //----------------------------------------------------------------------------------------------------------------------
    inline void set_z_axis(ngl::Vector _v){m_vecZ = _v;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the X axis
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector get_x_axis(){return m_vecX;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the Y axis
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector get_y_axis(){return m_vecY;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the Z axis
    //----------------------------------------------------------------------------------------------------------------------
    inline const ngl::Vector get_z_axis(){return m_vecZ;}


protected :
    /// @brief pointer to my Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief global scale of the axes
    //----------------------------------------------------------------------------------------------------------------------
    float m_scale;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief direction vector of X-Axis
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_vecX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief direction vector of Y-Axis
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_vecY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief direction vector of Z-Axis
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_vecZ;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief local transformation stack
    //----------------------------------------------------------------------------------------------------------------------
    ngl::TransformStack m_tx;

};

#endif // AXIS_H