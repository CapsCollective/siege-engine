#ifndef A_DARK_DISCOMFORT_SPLASHSCREEN_H
#define A_DARK_DISCOMFORT_SPLASHSCREEN_H

#include "../systems/entity/Entity2D.h"

class SplashScreen : public Entity2D
{
public:

    // Constructors

    SplashScreen() :
    logoPositionX(GetScreenWidth()/2 - 128),
    logoPositionY(GetScreenHeight()/2 - 128),
    framesCounter(0),
    lettersCount(0),
    topSideRecWidth(16),
    leftSideRecHeight(16),
    bottomSideRecWidth(16),
    rightSideRecHeight(16),
    state(BLINK),
    alpha(1.f),
    logoTexture()
    {
        Entity::SetName("SplashScreen");
    };

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

private:

    // Private enums

    enum IntroSequenceState { BLINK, BARS_1, BARS_2, LETTERING, CAPS_1, CAPS_2, END };

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

    Texture2D logoTexture;

};


#endif //A_DARK_DISCOMFORT_SPLASHSCREEN_H
