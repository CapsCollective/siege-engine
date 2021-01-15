#include "IntroSequence.h"
#include "../utils/SceneLoader.h"

void IntroSequence::OnUpdate()
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
            topSideRecWidth += 4;
            leftSideRecHeight += 4;
            if (topSideRecWidth == 256) state = BARS_2;
            break;
        case BARS_2:
            // Grow the bottom and right bars to size
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = LETTERING;
            break;
        case LETTERING:
            // Add new letters at framerate
            framesCounter++;
            if (framesCounter/12) lettersCount++, framesCounter = 0;

            // Fade out the lettering
            if (lettersCount >= 10)
            {
                alpha -= 0.02f;
                if (alpha <= -1.0f) alpha = 0.0f, state = END;
            }
            break;
        case END:
            // Load the main scene
            SceneLoader::QueueNextScene("main");
            break;
    }
}

void IntroSequence::OnUIDraw()
{
    // Run the appropriate draw calls for the animation state
    switch (state)
    {
        case BLINK:
            if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            break;
        case BARS_2:
            DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
            DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
        case BARS_1:
            DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
            DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
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
        case END:
            break;
    }
}
