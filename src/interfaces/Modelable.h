#ifndef A_DARK_DISCOMFORT_MODELABLE_H
#define A_DARK_DISCOMFORT_MODELABLE_H

#include "../resources/ModelData.h"

class Modelable
{
public:

    virtual const ModelData& GetModelData() = 0;

    virtual void SetModelData(const ModelData& modelData) = 0;

};


#endif //A_DARK_DISCOMFORT_MODELABLE_H
