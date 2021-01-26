#ifndef A_DARK_DISCOMFORT_TOOL_H
#define A_DARK_DISCOMFORT_TOOL_H

#include "Entity.h"

/**
 * An entity class for defining engine tools
 */
class Tool : public Entity
{
public:

    /**
     * A blank override to prevent freeing
     */
    void QueueFree() override {};
};


#endif //A_DARK_DISCOMFORT_TOOL_H
