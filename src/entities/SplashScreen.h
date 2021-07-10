#ifndef A_DARK_DISCOMFORT_SPLASHSCREEN_H
#define A_DARK_DISCOMFORT_SPLASHSCREEN_H

#include <entity/Entity.h>

enum IntroSequenceState {
    BLINK,
    BARS_1,
    BARS_2,
    LETTERING,
    CAPS_1,
    CAPS_2,
    END
};

class SplashScreen : public Entity
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    SplashScreen() :
        Entity(ENTITY_NAME),
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
    {};

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw2D() override;

private:

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
