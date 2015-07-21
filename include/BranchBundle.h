#ifndef BRANCHBUNDLE_H
#define BRANCHBUNDLE_H



#include <QTime>
#include "Bundle.h"

/// @file BranchBundle.h
/// @brief A form of Bundle which is to be used in tree structures.
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/2012
/// Revision History :
/// Initial Version 29/03/2012
/// @class BranchBundle
/// @brief This Bundle differs by having a start and end position which it blends between over a period of time.
class BranchBundle : public Bundle
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Constructor for BranchBundle
    /// @param [in] _id unique id integer
    /// @param [in] _startPos position vector for the start position
    /// @param [in] _endPos position vector for the end position
    /// @param [in] _startFrame the frame to start on
    /// @param [in] _frames the number of frames the animation will last
    /// @param [in] _parent the parent RibbonManager.
    //----------------------------------------------------------------------------------------------------------------------
    BranchBundle(
                const int _id,
                 const ngl::Vector &_startPos,
                 const ngl::Vector &_endPos,
                 const float &_startFrame,
                 const float &_frames,
                 RibbonManager *_parent
                 );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief calculate forces and positions
    //----------------------------------------------------------------------------------------------------------------------
    void update();

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief get the current rotation quaternion
    //----------------------------------------------------------------------------------------------------------------------
    inline ngl::Quaternion getRotQuat(){return m_rotQuat;}


protected:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the start position
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_startPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the end position
    //----------------------------------------------------------------------------------------------------------------------
    ngl::Vector m_endPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief frames the animation will last
    //----------------------------------------------------------------------------------------------------------------------
    float m_frames;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief frame on which to trigger the beginning of the animation
    //----------------------------------------------------------------------------------------------------------------------
    float m_startFrame;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief boolean to determine whether is animating (active)
    //----------------------------------------------------------------------------------------------------------------------
    bool ACTIVE;
};

#endif // BRANCHBUNDLE_H
