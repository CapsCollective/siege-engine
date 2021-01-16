#ifndef A_DARK_DISCOMFORT_MODELABLE_H
#define A_DARK_DISCOMFORT_MODELABLE_H

#include "../resource_system/ModelData.h"

class Modelable
{
public:

    // TODO reposition interface as Drawable & comment this class

    virtual const ModelData& GetModelData() = 0;

    virtual void SetModelData(const ModelData& modelData) = 0;

};


#endif //A_DARK_DISCOMFORT_MODELABLE_H
