#include <iostream>
#include "raylib.h"

#include "config.h"
#include "ego.h"
#include "setup.h"
#include "movement.h"
#include "drawing.h"


int main()
{
    InitWindow(config::env::screen::width_px, config::env::screen::height_px, "CarSimTest");
    SetTargetFPS(config::env::fps);

    ego::Ego ego;

    Camera2D camera = setup::setupCamera(ego);

    while (!WindowShouldClose())
    {
        movement::handleEgoInput(ego);
        movement::handleCameraFollow(camera, ego, false);
        movement::handleEgoMovement(ego);

        BeginDrawing();
        {
            BeginMode2D(camera);  // World coordinates
            {
                drawing::worldCoords::drawBackgroundScreen();
                drawing::worldCoords::drawEgo(ego);
                drawing::worldCoords::drawInfo(ego);
            }
            EndMode2D();

            // Screen Coordinates
            {
                drawing::screenCoords::drawInfo(ego);

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                {
                    Vector2 mousePos_px = GetMousePosition();
                    DrawCircle((int)mousePos_px.x, (int)mousePos_px.y, 30, RED); 
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
