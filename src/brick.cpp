#include "brick.h"
#include "game.h"
#include <raylib.h>

Brick::Brick(Game* game, float x, float y, Color color, bool attached)
    : IPhysicsBody(game)
    , color(color)
    , attached(attached)
{
    // Create Box2D body (convert pixels to meters)
    b2BodyDef bodyDef = b2DefaultBodyDef();
    
    if (attached) {
        bodyDef.type = b2_staticBody;  // Static when attached
    } else {
        bodyDef.type = b2_dynamicBody;  // Dynamic when broken off
    }
    
    bodyDef.position = {x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER};
    bodyDef.linearDamping = 2.0f;   // High linear damping to slow down movement
    bodyDef.angularDamping = 3.0f;  // High angular damping to slow down rotation
    bodyDef.isAwake = true;
    bodyId = b2CreateBody(game->GetWorldId(), &bodyDef);
    
    // Create box shape (convert dimensions to meters)
    b2Polygon box = b2MakeBox(
        BRICK_WIDTH / Game::PIXELS_PER_METER, 
        BRICK_HEIGHT / Game::PIXELS_PER_METER
    );
    
    // Create shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.5f;
    shapeDef.material.restitution = 0.3f;
    shapeDef.enableHitEvents = true;  // Enable hit events for breaking
    shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
}

void Brick::Update() {
    // Nothing to do - physics handled by Box2D
}

void Brick::Render() const {
    // Get position from Box2D body (convert meters to pixels)
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    b2Rot rot = b2Body_GetRotation(bodyId);
    
    float x = pos.x * Game::PIXELS_PER_METER;
    float y = pos.y * Game::PIXELS_PER_METER;
    float angle = b2Rot_GetAngle(rot) * RAD2DEG;
    
    Rectangle rect = {
        x,
        y,
        BRICK_WIDTH * 2,
        BRICK_HEIGHT * 2
    };
    
    Vector2 origin = { BRICK_WIDTH, BRICK_HEIGHT };
    
    // Draw filled brick
    DrawRectanglePro(rect, origin, angle, color);
    
    // Draw border inset within the brick dimensions
    const float borderThickness = 2.0f;
    const float inset = borderThickness / 2.0f;
    
    // Calculate the four corners of the inset rectangle (local coordinates)
    float halfWidth = BRICK_WIDTH - inset;
    float halfHeight = BRICK_HEIGHT - inset;
    
    Vector2 corners[4] = {
        { -halfWidth, -halfHeight },  // Top-left
        {  halfWidth, -halfHeight },  // Top-right
        {  halfWidth,  halfHeight },  // Bottom-right
        { -halfWidth,  halfHeight }   // Bottom-left
    };
    
    // Rotate corners and translate to world position
    float angleRad = angle * DEG2RAD;
    float cosA = cosf(angleRad);
    float sinA = sinf(angleRad);
    
    Vector2 worldCorners[4];
    for (int i = 0; i < 4; i++) {
        worldCorners[i].x = x + corners[i].x * cosA - corners[i].y * sinA;
        worldCorners[i].y = y + corners[i].x * sinA + corners[i].y * cosA;
    }
    
    // Draw the four border lines
    Color borderColor = ColorBrightness(color, -0.3f);  // Darker shade of brick color
    for (int i = 0; i < 4; i++) {
        DrawLineEx(worldCorners[i], worldCorners[(i + 1) % 4], borderThickness, borderColor);
    }
}
