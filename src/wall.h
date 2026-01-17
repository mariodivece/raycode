#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include "brick.h"

class Game;

class Wall {
public:
    Wall(Game* game, float startX, float startY, int brickCount, bool horizontal, Color color);
    ~Wall() = default;

    void Update();
    void Render() const;
    void CheckForBreaks();  // Check if any bricks should detach

private:
    Game* game;
    std::vector<std::unique_ptr<Brick>> bricks;
    
    static constexpr float BREAK_THRESHOLD = 2.0f;  // Velocity threshold for breaking
};
