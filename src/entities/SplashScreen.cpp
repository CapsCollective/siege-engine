#include "SplashScreen.h"
#include "../systems/scene/SceneManager.h"
#include "../systems/scene/SceneSerialiser.h"

// Define macros
#define CAPS_COLOUR CLITERAL(Color){ 166, 226, 209, 255 }

// Static member initialisation
const std::string SplashScreen::ENTITY_NAME("SplashScreen");

void SplashScreen::OnUpdate()
{
    // Run the state machine update
    switch (state)
    {
        case BLINK:
            // Blink the cursor at framerate
            framesCounter++;
            if (framesCounter == 120) framesCounter = 0, state = BARS_1;
            break;
        case BARS_1:
            // Grow the top and left bars to size
            alpha -= 0.02f;
            topSideRecWidth += 4;
            leftSideRecHeight += 4;
            if (topSideRecWidth == 256) state = BARS_2;
            break;
        case BARS_2:
            // Grow the bottom and right bars to size
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) alpha = 1.f, state = LETTERING;
            break;
        case LETTERING:
            // Add new letters at framerate
            framesCounter++;
            if (framesCounter/12) lettersCount++, framesCounter = 0;

            // Fade out the lettering
            if (lettersCount >= 10)
            {
                alpha -= 0.02f;
                if (alpha <= -1.0f)
                {
                    // Setup image for next state
                    Image image = LoadImage("assets/images/caps_collective.png");
                    logoTexture = LoadTextureFromImage(image);
                    UnloadImage(image);
                    alpha = 0.0f;
                    state = CAPS_1;
                }
            }
            break;
        case CAPS_1:
            // Fade in the logo
            alpha += 0.02f;
            if (alpha >= 2.0f) state = CAPS_2;
            break;
        case CAPS_2:
            // Fade out the logo
            alpha -= 0.02f;
            if (alpha <= -1.0f) state = END;
            break;
        case END:
            // End the animations and load the main scene
            UnloadTexture(logoTexture);
            SceneManager::QueueNextScene("main");
            break;
    }

    // Skip the playback on escape pressed
    if (IsKeyPressed(KEY_ESCAPE)) state = END;
}

void SplashScreen::OnDraw2D()
{
    // Run the appropriate draw calls for the animation state
    switch (state)
    {
        case BLINK:
            if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            DrawText("Press ESC to skip",10,GetScreenHeight() - 30,
                     20, Fade(BLACK, alpha));
            break;
        case BARS_2:
            DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
            DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
        case BARS_1:
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
            DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            DrawText("Press ESC to skip",10,GetScreenHeight() - 30,
                     20, Fade(BLACK, alpha));
            break;
        case LETTERING:
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
            DrawRectangle(logoPositionX, logoPositionY + 16, 16,
                          leftSideRecHeight - 32, Fade(BLACK, alpha));

            DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16,
                          rightSideRecHeight - 32, Fade(BLACK, alpha));
            DrawRectangle(logoPositionX, logoPositionY + 240,
                          bottomSideRecWidth, 16, Fade(BLACK, alpha));

            DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112,
                          224, 224, Fade(RAYWHITE, alpha));
            DrawText(TextSubtext("raylib", 0, lettersCount),GetScreenWidth()/2 - 44,
                     GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));
            break;
        case CAPS_1:
        case CAPS_2:
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(CAPS_COLOUR, alpha));
            DrawTexture(logoTexture, GetScreenWidth()/2 - logoTexture.width/2,
                        GetScreenHeight()/2 - logoTexture.height/2, ColorAlpha(WHITE, alpha-0.2f));
        case END:
            break;
    }
}

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& arg)
{
        return new SplashScreen();
}

REGISTER_SERIALISATION_INTERFACE(SplashScreen::ENTITY_NAME, nullptr, Deserialise);
