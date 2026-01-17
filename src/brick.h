#pragma once
#include <raylib.h>
#include <box2d/box2d.h>

class Game;

class Brick {
public:
    Brick(Game* game, float x, float y, Color color, bool attached = true);
    ~Brick();

    void Update();
    void Render() const;
    bool IsAttached() const { return attached; }
    void Detach() { attached = false; }
    b2BodyId GetBodyId() const { return bodyId; }
    b2ShapeId GetShapeId() const { return shapeId; }

private:
    Game* game;
    b2BodyId bodyId;
    b2ShapeId shapeId;
    Color color;
    bool attached;  // Whether brick is still attached to wall
    
    static constexpr float BRICK_WIDTH = 7.5f;   // Half ball radius
    static constexpr float BRICK_HEIGHT = 7.5f;  // Half ball radius
};
