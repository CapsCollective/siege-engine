#ifndef A_DARK_DISCOMFORT_CLONEABLE_H
#define A_DARK_DISCOMFORT_CLONEABLE_H

class Cloneable
{
public:

    /**
     * A virtual method for implementing more complex object
     * copying logic
     * @return A pointer to the cloned entity object
     */
    virtual Entity* Clone() = 0;

};

#endif //A_DARK_DISCOMFORT_CLONEABLE_H
