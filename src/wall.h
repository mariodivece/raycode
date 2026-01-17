#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include "brick.h"
#include "IRenderable.h"

class Game;

class Wall : public IRenderable {
public:
    Wall(Game* game, float startX, float startY, int brickCount, bool horizontal, Color color);
    ~Wall() override = default;

    void Update();
    void Render() const override;
    void CheckForBreaks();  // Check if any bricks should detach

private:
    Game* game;
    std::vector<std::unique_ptr<Brick>> bricks;
    
    static constexpr float BREAK_THRESHOLD = 2.0f;  // Velocity threshold for breaking
};
