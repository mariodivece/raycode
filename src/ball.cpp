#include "ball.h"
#include "game.h"
#include "FakeLight.h"
#include <raylib.h>
#include <raymath.h>

Ball::Ball(Game* game, bool autoBounce)
    : IPhysicsBody(game)
    , radius(15)
    , color(WHITE)
    , isPlayer(!autoBounce)
{
    // Create Box2D body (convert pixels to meters)
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {100.0f / Game::PIXELS_PER_METER, 100.0f / Game::PIXELS_PER_METER};
    bodyDef.linearDamping = 0.5f;  // Add some friction
    bodyDef.isAwake = true;  // Ensure body starts awake
    bodyId = b2CreateBody(game->GetWorldId(), &bodyDef);
    
    // Create circle shape (convert radius to meters)
    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = radius / Game::PIXELS_PER_METER;
    
    // Create shape definition with random restitution (bounciness)
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.isSensor = false;  // NOT a sensor - solid collision
    shapeDef.material.friction = 0.3f;
    // Random restitution between 0.7 and 0.9
    shapeDef.material.restitution = 0.7f + (float)(GetRandomValue(0, 20)) / 100.0f;
    b2CreateCircleShape(bodyId, &shapeDef, &circle);
    
    // If auto-bounce (enemy), give initial velocity
    if (autoBounce) {
        float vx = (float)(GetRandomValue(-50, 50));
        float vy = (float)(GetRandomValue(-50, 50));
        b2Body_SetLinearVelocity(bodyId, {vx, vy});
    }
}

Ball::Ball(Game* game, float x, float y, Color color, bool autoBounce)
    : IPhysicsBody(game)
    , radius(15)
    , color(color)
    , isPlayer(!autoBounce)
{
    // Create Box2D body (convert pixels to meters)
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {x / Game::PIXELS_PER_METER, y / Game::PIXELS_PER_METER};
    bodyDef.linearDamping = 0.5f;
    bodyDef.isAwake = true;  // Ensure body starts awake
    bodyId = b2CreateBody(game->GetWorldId(), &bodyDef);
    
    // Create circle shape (convert radius to meters)
    b2Circle circle{};
    circle.center = {0.0f, 0.0f};
    circle.radius = radius / Game::PIXELS_PER_METER;
    
    // Create shape definition with random restitution
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.isSensor = false;  // NOT a sensor - solid collision
    shapeDef.material.friction = 0.3f;
    // Random restitution between 0.7 and 0.9
    shapeDef.material.restitution = 0.7f + (float)(GetRandomValue(0, 20)) / 100.0f;
    b2CreateCircleShape(bodyId, &shapeDef, &circle);
    if (autoBounce) {
        float vx = (float)(GetRandomValue(-50, 50));
        float vy = (float)(GetRandomValue(-50, 50));
        b2Body_SetLinearVelocity(bodyId, {vx, vy});
    }
}

void Ball::Update() {
    // Nothing to do - physics is handled by Box2D
    // Position is automatically updated by the physics engine
}

void Ball::Render() const {
    // Get position from Box2D body (convert meters to pixels)
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    Vector2 position = { pos.x * Game::PIXELS_PER_METER, pos.y * Game::PIXELS_PER_METER };
    
    // Get light intensity at this position
    FakeLight* light = game->GetLight();
    float intensity = light ? light->GetIntensityAt(position) : 1.0f;
    
    // Apply intensity to base color (darker when further from light)
    Color litColor = ColorBrightness(color, (intensity - 1.0f) * 0.5f);
    Color edgeColor = ColorBrightness(litColor, -0.3f);  // Darker at edges
    
    // Calculate highlight offset based on light direction  
    Vector2 gradientCenter = position;
    if (light) {
        Vector2 highlightOffset = light->GetHighlightOffset(position, radius);
        // Invert the offset so bright spot faces the light
        gradientCenter.x = position.x - highlightOffset.x;
        gradientCenter.y = position.y - highlightOffset.y;
    }
    
    // Draw smooth gradient from bright center to darker edge
    Color centerColor = ColorBrightness(litColor, 0.4f);  // Brighter at highlight
    DrawCircleGradient(
        (int)gradientCenter.x,
        (int)gradientCenter.y,
        radius,
        centerColor,
        edgeColor
    );
    
    // Add subtle specular highlight for extra shine
    if (light) {
        Vector2 highlightOffset = light->GetHighlightOffset(position, radius);
        Vector2 specularPos = {
            position.x + highlightOffset.x * 0.6f,
            position.y + highlightOffset.y * 0.6f
        };
        float specularRadius = radius * 0.2f;
        DrawCircleV(specularPos, specularRadius, ColorAlpha(WHITE, 0.4f));
    }
}

void Ball::ApplyForce(float x, float y) {
    b2Vec2 force = {x * MOVE_FORCE, y * MOVE_FORCE};
    b2Body_ApplyForceToCenter(bodyId, force, true);
}
