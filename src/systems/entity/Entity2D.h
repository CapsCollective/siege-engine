#ifndef A_DARK_DISCOMFORT_ENTITY2D_H
#define A_DARK_DISCOMFORT_ENTITY2D_H

#include "Entity.h"

class Entity2D : public Entity
{
public:

    // Virtual methods

    /**
     * A virtual update method to be overridden for widget
     * drawing instructions
     */
    virtual void OnDraw() {};

protected:

    // Constructors

    /**
     * Zero-param constructor for Widget, initialises
     * z-index to zero
     */
    Entity2D() :
    Entity2D(0)
    {};

    /**
     * Delegate constructor for Widget
     * @param zIndex
     */
    explicit Entity2D(int zIndex) :
    zIndex(zIndex)
    {};

    // Protected methods

    void SetZIndex(int idx);

    // Protected fields

    /**
     * The widget z-index for render order
     */
    int zIndex;

};


#endif //A_DARK_DISCOMFORT_ENTITY2D_H
