#include <vector>
#include <iostream>

#include "raylib.h"
#include "raymath.h"

const int screenWidth = 2560;
const int screenHeight = 1440;
const int target_fps = 60;
float time_warp = 1;

#include "Objects.h"
#include "Camera.h"
#include "Simulation_functions.h"
#include "User_interface.h"

int main(void)
{
    //  window init

    InitWindow(screenWidth, screenHeight, "Orbital_transfer_simulator");
    ToggleBorderlessWindowed();

    SetTargetFPS(target_fps);


    //  camera init

    Custom_camera camera;


    //  simulation init

    std::vector<Planet> planets;
    planetary_bodies_init(planets);
    size_t planets_num = planets.size();

    Ship ship({ 0.0f, -2800.0f },
        { 0.1514f, 0.0f },
        ORANGE
    );

    while (!WindowShouldClose())
    {
        //  time-related functions

        float delta_time = GetFrameTime() * time_warp;

        //  Forces update

        gravitational_attraction(planets, planets_num, ship);
        ship.ship_controls();

        //  Kinematics update

        for (size_t j = 0; j < planets_num; j++)
        {
            planets[j].update(delta_time);
        }

        ship.update(delta_time);

        //  Logic update

        if (IsKeyPressed(KEY_TWO) && time_warp > 0.1f)    time_warp /= 10;
        if (IsKeyPressed(KEY_THREE) && time_warp < 10000.0f) time_warp *= 10;

        camera.camera_controls(ship);

        //  Drawing

        BeginDrawing();
        {

            ClearBackground({ 10, 10, 20, 255 });

            //  Drawing moving elements

            BeginMode2D(camera.camera);
            {
                DrawGrid2D(camera.camera, 1000.0f, DARKGRAY);
                
                draw_planet_trajectory(planets, planets_num, { 1 }, 20000, ship);

                for (size_t i = 0; i < planets_num; i++)
                {
                    planets[i].display_object();
                }
                ship.display_object();


            }
            EndMode2D();

            //  Drawing UI

            draw_ui(camera.zoom_mode, ship);
        }
        EndDrawing();
    }

    CloseWindow();


    return 0;
}