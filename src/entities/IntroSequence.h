#ifndef A_DARK_DISCOMFORT_INTROSEQUENCE_H
#define A_DARK_DISCOMFORT_INTROSEQUENCE_H

#include "Entity.h"

class IntroSequence : public Entity
{
public:

    // Constructors

    IntroSequence() :
    logoPositionX(GetScreenWidth()/2 - 128),
    logoPositionY(GetScreenHeight()/2 - 128),
    framesCounter(0),
    lettersCount(0),
    topSideRecWidth(16),
    leftSideRecHeight(16),
    bottomSideRecWidth(16),
    rightSideRecHeight(16),
    state(BLINK),
    alpha(1.f)
    {
        Entity::SetName("IntroSequence");
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnUIDraw() override;

private:

    // Private enums

    enum IntroSequenceState { BLINK = 0, BARS_1 = 1, BARS_2 = 2, LETTERING = 3, END = 4 };

    // Private fields

    int logoPositionX;
    int logoPositionY;

    int framesCounter;
    int lettersCount;

    int topSideRecWidth;
    int leftSideRecHeight;

    int bottomSideRecWidth;
    int rightSideRecHeight;

    IntroSequenceState state;
    float alpha;

};


#endif //A_DARK_DISCOMFORT_INTROSEQUENCE_H
