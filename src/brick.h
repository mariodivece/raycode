#pragma once
#include <raylib.h>
#include <box2d/box2d.h>
#include "IPhysicsBody.h"
#include "IRenderable.h"

class Game;

class Brick : public IPhysicsBody, public IRenderable {
public:
    Brick(Game* game, float x, float y, Color color, bool attached = true);
    ~Brick() override = default;

    void Update() override;
    void Render() const override;
    bool IsAttached() const { return attached; }
    void Detach() { attached = false; }
    b2ShapeId GetShapeId() const { return shapeId; }

private:
    b2ShapeId shapeId;
    Color color;
    bool attached;  // Whether brick is still attached to wall
    
    static constexpr float BRICK_WIDTH = 7.5f;   // Half ball radius
    static constexpr float BRICK_HEIGHT = 7.5f;  // Half ball radius
};
