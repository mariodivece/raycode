#pragma once

#include <raylib.h>
#include <memory>
#include <vector>
#include <box2d/box2d.h>

class Ball;
class Wall;

class Game {
public:
    Game();
    ~Game();

    void Update();
    void Render();
    bool IsRunning() const;

    float GameTime() const;
    double ElapsedTime() const;
    void RequestExit();
    Color GetBackgroundColor() const;
    int TargetFps() const;
    void TargetFps(int value);
    float ScreenWidth() const;
    void ScreenWidth(float value);
    float ScreenHeight() const;
    void ScreenHeight(float value);
    void ProcessInput();
    
    b2WorldId GetWorldId() const { return worldId; }
    
    // Box2D works best with meter-based units (0.1 to 10 meters)
    // Scale factor: 1 meter = 50 pixels
    static constexpr float PIXELS_PER_METER = 50.0f;

private:
    bool running;
    bool exitRequested = false;
    int targetFps = 60;
    float screenWidth = 800;
    float screenHeight = 600;
    b2WorldId worldId;
    b2BodyId wallBodies[4];  // Top, bottom, left, right walls
    std::unique_ptr<Ball> player;
    std::vector<std::unique_ptr<Ball>> enemies;
    std::vector<std::unique_ptr<Wall>> walls;
    
    void CreateWorldBounds();
};
