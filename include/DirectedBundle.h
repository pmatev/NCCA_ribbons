#ifndef DIRECTEDBUNDLE_H
#define DIRECTEDBUNDLE_H

#include "Bundle.h"

/// @file DirectedBundle.h
/// @brief A type of Bundle which moves relatively freely with user control over position.
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/2012
/// Revision History :
/// Initial Version 29/03/2012
/// @class DirectedBundle
/// @brief A type of Bundle which has user controlled movement.
class DirectedBundle : public Bundle
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief DirectedBundle Constructor
    /// @param [in] _id unique id
    /// @param [in] _startPos position to start from
    /// @param [in] _vel start velocity
    /// @param [in] _parent RibbonManager parent
    //----------------------------------------------------------------------------------------------------------------------
    DirectedBundle(const int &_id, const ngl::Vector &_startPos, const ngl::Vector &_vel, RibbonManager *_parent);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief calculate forces and update position
    //----------------------------------------------------------------------------------------------------------------------
    void update();


};

#endif // DIRECTEDBUNDLE_H
