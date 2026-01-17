#pragma once
#include <raylib.h>
#include <box2d/box2d.h>

class Game;

class Ball {
public:
    Ball(Game* game, bool autoBounce = true);
    Ball(Game* game, float x, float y, Color color, bool autoBounce = true);
    ~Ball() = default;

    void Update();
    void Render() const;
    void ApplyForce(float x, float y);

private:
    Game* game;
    b2BodyId bodyId;
    float radius;
    Color color;
    bool isPlayer;

    static constexpr float MOVE_FORCE = 50.0f;
};
