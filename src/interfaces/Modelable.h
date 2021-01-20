#ifndef A_DARK_DISCOMFORT_MODELABLE_H
#define A_DARK_DISCOMFORT_MODELABLE_H

#include "../systems/resource/ModelData.h"

class Modelable
{
public:

    /**
     * A virtual getter method model data
     * @return The object's model data
     */
    virtual const ModelData& GetModelData() = 0;

    /**
     * A virtual setter method model data
     * @param modelData - model data to set to the
     *                    object
     */
    virtual void SetModelData(const ModelData& modelData) = 0;

};


#endif //A_DARK_DISCOMFORT_MODELABLE_H
