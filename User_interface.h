#pragma once

void draw_ui(bool zoomMode, Ship ship)
{
    int font_size   = 20; // px
    int x_offset    = 20; // px

    DrawFPS(20, 20);
    if (zoomMode == 0)  DrawText("LMB drag to move, MW to zoom", x_offset, 50, font_size, LIGHTGRAY);
    else                DrawText("LMB drag to move, RMB press and move to zoom", x_offset, 50, font_size, LIGHTGRAY);
    DrawText("[1] Select mouse zoom mode", x_offset, 80, font_size, LIGHTGRAY);
    DrawText(TextFormat("[2] [3] Time warp set to %.1f X", time_warp), x_offset, 110, font_size, LIGHTGRAY);
    DrawText("[4] Lock camera on ship", x_offset, 140, font_size, LIGHTGRAY);
    DrawText(TextFormat("[5] Cycle ship orientation: %i", ship.target_direction), x_offset, 170, font_size, LIGHTGRAY);
    //DrawText(TextFormat("Ship vel x: %.5f", ship.velocity.x), x_offset, 170, font_size, LIGHTGRAY);
    //DrawText(TextFormat("Ship vel y: %.5f", ship.velocity.y), x_offset, 200, font_size, LIGHTGRAY);
}

void DrawGrid2D(Camera2D camera, float base_spacing, Color color)
{
    float zoom = camera.zoom;

    float spacing = base_spacing;

    while (spacing * zoom < 160.0f)
    {
        spacing *= 5.0f;
    }

    Vector2 topLeft = GetScreenToWorld2D(
        { 0, 0 }, camera
    );

    Vector2 bottomRight = GetScreenToWorld2D(
        { (float)screenWidth, (float)screenHeight },
        camera
    );

    int startX = (int)floor(topLeft.x / spacing);
    int endX = (int)ceil(bottomRight.x / spacing);

    int startY = (int)floor(topLeft.y / spacing);
    int endY = (int)ceil(bottomRight.y / spacing);

    for (int x = startX; x <= endX; x++)
    {
        float worldX = x * spacing;

        DrawLineV(
            { worldX, topLeft.y },
            { worldX, bottomRight.y },
            color
        );
    }

    for (int y = startY; y <= endY; y++)
    {
        float worldY = y * spacing;

        DrawLineV(
            { topLeft.x, worldY },
            { bottomRight.x, worldY },
            color
        );
    }
}

void draw_planet_trajectory(std::vector<Planet> planets, int size, std::vector<int> planets_indexes, int time_steps, Ship ship)
{
    float resolution = 5.0f; // s

    int size_2 = planets_indexes.size();

    std::vector<Vector2> previous_positions(size_2);
    for (size_t i = 0; i < size_2; i++)
    {
        previous_positions[i] = planets[planets_indexes[i]].position;
    }

    Vector2 previous_pos_ship = ship.position;


    for (size_t i = 0; i < time_steps; i++)
    {
        gravitational_attraction(planets, size, ship);

        for (size_t j = 0; j < size; j++)
        {
            planets[j].update(resolution);
        }
        ship.update(resolution);

        DrawLineEx(previous_pos_ship, ship.position, 5.0f, WHITE);
        previous_pos_ship = ship.position;

        for (size_t k = 0; k < size_2; k++)
        {
            DrawLineEx(previous_positions[k], planets[planets_indexes[k]].position, 5.0f, WHITE);
            previous_positions[k] = planets[planets_indexes[k]].position;
        }
    }
}