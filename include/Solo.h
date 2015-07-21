#ifndef SOLO_H
#define SOLO_H


#include "Bundle.h"

/// @file Solo.h
/// @brief The first bundle that is created when the program runs. It handles singular ribbons with minimal user interaction.
/// @author Peter Matev
/// @version 1.0
/// @date 29/03/2012
/// Revision History :
/// Initial Version 29/03/2012
/// @class Solo
/// @brief The first bundle that is created when the program runs. It handles singular ribbons with minimal user interaction.
class Solo : public Bundle
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Solo Constructor
    //----------------------------------------------------------------------------------------------------------------------
    Solo(int _id, RibbonManager *_parent);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief update Solo
    //----------------------------------------------------------------------------------------------------------------------
    void update();

protected:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief states available in Solo ribbon mode /* NOT IMPLEMENTED */
    //----------------------------------------------------------------------------------------------------------------------
    enum STATE {NORMAL, LOOP, SPIN, SHIVER};



};

#endif // SOLO_H
