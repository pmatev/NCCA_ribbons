#ifndef CAMERA_H
#define CAMERA_H



#include <ngl/Camera.h>
#include <ngl/TransformStack.h>

class GLWindow;
class Bundle;

/// @file Camera.h
/// @brief An extension of the ngl::Camera class to provide smooth navigation.
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/2012
/// Revision History :
/// Initial Version 29/03/2012
/// @class Camera
/// @brief Extends the functionality of the ngl::Camera to provide dolly, track and tumble movement using quaternions.
/// Also provides a mechanism for following a Bundle and tumbling around it.
class Camera : public ngl::Camera {


public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for Camera
    /// @param [in] _from eye vector
    /// @param [in] _to look vector
    /// @param [in] _up the up vector
    /// @param [in] _parent parent GLWindow
    //----------------------------------------------------------------------------------------------------------------------
    Camera(const ngl::Vector &_from, const ngl::Vector &_to, const ngl::Vector &_up, GLWindow *_parent);
    //Camera(Bundle *_parentBundle);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Destructor for Camera
    //----------------------------------------------------------------------------------------------------------------------
    ~Camera();


    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update the eye and look vectors
    //----------------------------------------------------------------------------------------------------------------------
    void update();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the eye and look vectors to the ngl::Camera
    //----------------------------------------------------------------------------------------------------------------------
    void setNew();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function to tumble the camera around the look position using quaternion rotations
    /// @param [in] _oldX previous mouse position X coord
    /// @param [in] _oldX previous mouse position Y coord
    /// @param [in] _newX mouse position X coord
    /// @param [in] _newX mouse position Y coord
    //----------------------------------------------------------------------------------------------------------------------
    void tumble(int _oldX, int _oldY, int _newX, int _newY);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function to track the camera
    /// @param [in] _oldX previous mouse position X coord
    /// @param [in] _oldX previous mouse position Y coord
    /// @param [in] _newX mouse position X coord
    /// @param [in] _newX mouse position Y coord
    //----------------------------------------------------------------------------------------------------------------------
    void track(int _oldX, int _oldY, int _newX, int _newY);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief function to dolly the camera in and out along the viewing normal
    /// @param [in] _oldX previous mouse position X coord
    /// @param [in] _newX mouse position X coord
    //----------------------------------------------------------------------------------------------------------------------
    void dolly(int _oldX, int _newX);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the Bundle to follow and start following
    /// @param _target pointer to the bundle which to follow
    //----------------------------------------------------------------------------------------------------------------------
    inline void follow(Bundle *_target){FOLLOW=true; m_target = _target;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief turn the FOLLOW functionality off and revert to regular navigation
    //----------------------------------------------------------------------------------------------------------------------
    inline void followNone(){FOLLOW = false;}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief return the Bundle which is being followed by the camera
    //----------------------------------------------------------------------------------------------------------------------
    inline Bundle* getFollowTarget(){return m_target;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief set the radius of the sphere which the camera eye can move on
    //----------------------------------------------------------------------------------------------------------------------
    inline void setRadius(float _z){m_radius = _z;}
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the radius of the sphere which the camera eye can move on
    //----------------------------------------------------------------------------------------------------------------------
    inline float getRadius(){return m_radius;}

protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief parent GLWindow
    //----------------------------------------------------------------------------------------------------------------------
    GLWindow *m_parent;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief state switching boolean to follow Bundle
    //----------------------------------------------------------------------------------------------------------------------
    bool FOLLOW;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief target to follow
    //----------------------------------------------------------------------------------------------------------------------
    Bundle *m_target;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief tumble velocity - for momentum tumble /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    float m_tumbleVel;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief tumble angle - for momentum tumble /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    float m_tumbleAngle;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief radius of the sphere in screen space
    //----------------------------------------------------------------------------------------------------------------------
    float m_radius;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the axis about which to rotate when tumbling
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_rotAxis;
};
#endif // CAMERA_H
