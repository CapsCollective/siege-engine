#ifndef A_DARK_DISCOMFORT_GRID_H
#define A_DARK_DISCOMFORT_GRID_H

#include "../../systems/entity/Entity3D.h"

class Grid : public Entity3D
{
public:

    explicit Grid(int size = 100.f, bool isActive = true) :
    isActive(isActive),
    size(size)
    {};

    // Public methods

    void SetActive(bool active);

    bool GetActive() const;

protected:

    // Protected overrides

    void OnDraw() override;

private:

    // Private fields

    bool isActive;

    int size;
};


#endif //A_DARK_DISCOMFORT_GRID_H
