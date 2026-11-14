#pragma once

class Custom_camera
{
private:

    void camera_init()
    {
        camera.zoom = 0.4f;
        camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    }

public:

    Camera2D camera = { 0 };
    bool zoom_mode = 0;
    bool camera_follows_ship = 0;

    Custom_camera()
    {
        camera_init();
    }
    ~Custom_camera()
    {
    }

    void camera_controls(Ship ship)
    {
        const float min_zoom = 0.0000125f;
        const float max_zoom = 50.0f;

        if (IsKeyPressed(KEY_ONE)) zoom_mode = !zoom_mode;
        if (IsKeyPressed(KEY_FOUR)) camera_follows_ship = !camera_follows_ship;

        if (camera_follows_ship)
        {
            camera.offset = { screenWidth / 2, screenHeight / 2 };
            camera.target = ship.position;

            float wheel = GetMouseWheelMove();
            float scale = 0.2f * wheel;
            camera.zoom = Clamp(expf(logf(camera.zoom) + scale), min_zoom, max_zoom);
        }

        else
        {

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                Vector2 delta = GetMouseDelta();
                delta = Vector2Scale(delta, -1.0f / camera.zoom);
                camera.target = Vector2Add(camera.target, delta);
            }

            if (zoom_mode == 0)
            {
                float wheel = GetMouseWheelMove();
                if (wheel != 0)
                {
                    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                    camera.offset = Vector2Multiply(GetMousePosition(), GetWindowScaleDPI());

                    camera.target = mouseWorldPos;

                    float scale = 0.2f * wheel;
                    camera.zoom = Clamp(expf(logf(camera.zoom) + scale), min_zoom, max_zoom);
                }
            }
            else
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                {
                    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
                    camera.offset = Vector2Multiply(GetMousePosition(), GetWindowScaleDPI());
                    camera.target = mouseWorldPos;
                }

                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                {
                    float deltaX = GetMouseDelta().x;
                    float scale = 0.005f * deltaX;
                    camera.zoom = Clamp(expf(logf(camera.zoom) + scale), min_zoom, max_zoom);
                }
            }
        }
    }
};
