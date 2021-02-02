#ifndef A_DARK_DISCOMFORT_COLLIDABLE_H
#define A_DARK_DISCOMFORT_COLLIDABLE_H

// TODO use this?
class Collidable
{
public:

    /**
     * A virtual method for implementing collision
     * handling logic, called on collision
     * @param entity - the colliding entity
     */
    virtual void OnCollision(class Entity* entity) = 0;

};

#endif //A_DARK_DISCOMFORT_COLLIDABLE_H
