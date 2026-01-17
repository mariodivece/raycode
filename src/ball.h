#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include "IPhysicsBody.h"
#include "IRenderable.h"

class Game;

class Ball : public IPhysicsBody, public IRenderable {
public:
    Ball(Game* game, bool autoBounce = true);
    Ball(Game* game, float x, float y, Color color, bool autoBounce = true);
    ~Ball() override = default;

    void Update() override;
    void Render() const override;
    void ApplyForce(float x, float y);

private:
    float radius;
    Color color;
    bool isPlayer;

    static constexpr float MOVE_FORCE = 50.0f;
};
