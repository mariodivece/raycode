#include "wall.h"
#include "brick.h"
#include "game.h"
#include <raylib.h>
#include <box2d/box2d.h>

Wall::Wall(Game* game, float startX, float startY, int brickCount, bool horizontal, Color color)
    : game(game)
{
    // Create bricks in a line
    for (int i = 0; i < brickCount; i++) {
        float x, y;
        if (horizontal) {
            x = startX + i * 15.0f;  // 15 pixels = brick width * 2
            y = startY;
        } else {
            x = startX;
            y = startY + i * 15.0f;  // 15 pixels = brick height * 2
        }
        
        bricks.push_back(std::make_unique<Brick>(game, x, y, color, true));
    }
}

void Wall::Update() {
    CheckForBreaks();
    
    for (auto& brick : bricks) {
        if (brick) brick->Update();
    }
}

void Wall::Render() const {
    for (const auto& brick : bricks) {
        if (brick) brick->Render();
    }
}

void Wall::CheckForBreaks() {
    // Get contact hit events from the world
    b2ContactEvents events = b2World_GetContactEvents(game->GetWorldId());
    
    // Check hit events (high velocity impacts)
    for (int i = 0; i < events.hitCount; i++) {
        const b2ContactHitEvent& hit = events.hitEvents[i];
        
        // Check if either shape belongs to one of our bricks
        for (auto& brick : bricks) {
            if (!brick || !brick->IsAttached()) continue;
            
            b2ShapeId brickShapeId = brick->GetShapeId();
            
            // Check if this hit involves our brick
            if (B2_ID_EQUALS(hit.shapeIdA, brickShapeId) || B2_ID_EQUALS(hit.shapeIdB, brickShapeId)) {
                // Break the brick!
                brick->Detach();
                
                // Change brick to dynamic body
                b2Body_SetType(brick->GetBodyId(), b2_dynamicBody);
                
                // Apply impulse based on impact direction
                b2Vec2 impulse = {
                    hit.normal.x * 1.0f,
                    hit.normal.y * 1.0f
                };
                b2Body_ApplyLinearImpulseToCenter(brick->GetBodyId(), impulse, true);
                
                break;
            }
        }
    }
}
